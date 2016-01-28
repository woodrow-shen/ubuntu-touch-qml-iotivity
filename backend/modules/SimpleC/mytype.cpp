#include <cstdlib>
#include <typeinfo>
#include "mytype.h"

// global
DiscoveredResourceMap discoveredResources;
std::shared_ptr<OCResource> ledResourceA = NULL;
std::mutex curResourceLock;

Launcher * Launcher::m_cur_launcher;

static FILE* client_open(const char* /*path*/, const char *mode)
{
    return fopen("./oic_svr_db_client.json", mode);
}

Launcher::Launcher(QObject *parent) :
    QObject(parent),
    m_message(""),
    m_process(new QProcess(this))
{
	m_cur_launcher = this;
}

Launcher *Launcher::getInstance()
{
	try 
	{
		typeid(m_cur_launcher);
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception in getInstance: " << e.what() << std::endl;
	}
	return m_cur_launcher;
}

Launcher::~Launcher() {

}

void Launcher::setIotStatus(QString msg)
{
	if (QString::compare(m_message, msg, Qt::CaseInsensitive))
	{
		m_message = msg; 
		emit iotStatusChanged(msg); 
	}
}

void onPutLedA(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	Launcher *launcher = Launcher::getInstance();
	launcher->m_output.str("");

	try 
	{
		if(eCode == OC_STACK_OK) 
		{
			launcher->m_output << "Arduino Led PUT request was successful" << std::endl;
			launcher->setIotStatus(QString::fromStdString(launcher->m_output.str()));
		} 
		else 
		{
			launcher->m_output << "onPutLed Response error: " << eCode << std::endl;
			launcher->setIotStatus(QString::fromStdString(launcher->m_output.str()));
			std::exit(-1);
		}
	}

	catch(std::exception& e) 
	{
		std::cerr << "Exception: " << e.what() << " in onPutLed" << std::endl;
	}  
}

// Callback to found resources
void foundResource(std::shared_ptr<OCResource> resource)
{
	Launcher *launcher = Launcher::getInstance();
	launcher->m_output.str("");
	launcher->m_output << "In foundResource" << std::endl;
	std::string resourceURI;
	std::string hostAddress;

	try
	{
		{
			std::lock_guard<std::mutex> lock(curResourceLock);
			if(discoveredResources.find(resource->uniqueIdentifier()) == discoveredResources.end())
			{   
				launcher->m_output << "Found resource " << resource->uniqueIdentifier() <<
					" for the first time on server with ID: "<< resource->sid()<<std::endl;
				discoveredResources[resource->uniqueIdentifier()] = resource;
			}   
			else
			{   
				launcher->m_output<<"Found resource "<< resource->uniqueIdentifier() << " again!"<<std::endl;
			} 
		}
	
		// Do some operations with resource object.
		if(resource) 
		{
       		launcher->m_output<<"DISCOVERED Resource:"<<std::endl;
			// Get the resource URI
			resourceURI = resource->uri();
			launcher->m_output << "\tURI of the resource: " << resourceURI << std::endl;

			// Get the resource host address
			hostAddress = resource->host();
			launcher->m_output << "\tHost address of the resource: " << hostAddress << std::endl;
			
			// Get the resource types
			launcher->m_output << "\tList of resource types: " << std::endl;
			for(auto &resourceTypes : resource->getResourceTypes()) 
			{
				launcher->m_output << "\t\t" << resourceTypes << std::endl;
			}

			// Get the resource interfaces
			launcher->m_output << "\tList of resource interfaces: " << std::endl;
			for(auto &resourceInterfaces : resource->getResourceInterfaces()) 
			{
				launcher->m_output << "\t\t" << resourceInterfaces << std::endl;
			}

			if(resourceURI == "/gateway/leda") 
			{
				if(ledResourceA) 
				{
					launcher->m_output << "Found another LED resource, ignoring" << std::endl;
				} 
				else 
				{
					launcher->m_output << "Find LED resource" << std::endl;
					ledResourceA = resource;
				}
			}

		}// if(resource)
		else
		{
			// Resource is invalid
			launcher->m_output << "Resource is invalid" << std::endl;
		}
		launcher->setIotStatus(QString::fromStdString(launcher->m_output.str()));
	}
	catch(std::exception& e) 
	{
		std::cerr << "Exception in foundResource: "<< e.what() << std::endl;
	}
}

QString Launcher::launch(const QString &program)
{
    m_process->start(program);
    m_process->waitForFinished(-1);
    QByteArray bytes = m_process->readAllStandardOutput();
    QString output = QString::fromLocal8Bit(bytes);

    return output;
}

void Launcher::init()
{
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Both,
        "0.0.0.0",
        0,
        OC::QualityOfService::LowQos,
        &ps
    };

//	QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
//	engine->rootContext()->setContextProperty("rootItem", (QObject *)this);

    OCPlatform::Configure(cfg);
	std::ostringstream requestURI;
	std::string led_p_rt = "?rt=gateway.leda";
	std::string resource_name1 = "Arduino";
	std::string resource_name2 = "RaspberryPi2";

	try
	{
		this->setIotStatus("OCPlatform::Configure");

		// makes it so that all boolean values are printed as 'true/false' in this stream
		m_output.setf(std::ios::boolalpha);

		requestURI.str("");
		m_output.str("");
		requestURI << OC_RSRVD_WELL_KNOWN_URI << led_p_rt;
		OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource);
		std::cout << resource_name1 << " Finding LED Resource... " << std::endl;	

		std::cout << "waiting for resource finding..." << std::endl;
		for(;!ledResourceA;) {}
		std::cout << "prepare to do operation..." << std::endl;
	}
	catch(OCException& e)
	{
		oclog() << "Exception in main: " << e.what();
	}
}

void Launcher::switchLed(bool checked)
{
	m_output.str("");
	m_output << "Putting Arduno LED representation..." << std::endl;
	this->setIotStatus(QString::fromStdString(m_output.str()));

	if(ledResourceA)
	{
		OCRepresentation rep;
		rep.setValue("status", (!checked) ? 0 : 255);
		ledResourceA->put(rep, QueryParamsMap(), &onPutLedA);
	}
}
