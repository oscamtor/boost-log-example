// #define BOOST_LOG_DYN_LINK 1                // <=> Establecerlo en CMAkeList.txt set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ...  -DBOOST_LOG_DYN_LINK)

#include <iostream>
#include <ctime>
#include <chrono>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include "boost/log/utility/setup.hpp" // For add_console_log (permitir salir tambien por consola)

// Podremos usar lo de la izda en lugar de lo de a la derecha
namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
namespace triv = boost::log::trivial;

static void init_log()
{
    // Solo usada en Opcion 1 Basica
	static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

    boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");

    // Output message to console
    boost::log::add_console_log(
        std::cout,
		
		// Fuerzo el mismo formato que a fichero!. Elegir en ambos add_console_log() y add_file_log() un mismo keywords::format sea el que sea:
		// Opcion 1 Basica
	    // Output:
		// ----------------------------------------------------------------------
		// [2022-03-17 08:08:32.277274][info]:  An informational severity message
		// ----------------------------------------------------------------------
		keywords::format = COMMON_FMT,
		
		// Opcion 2 inpa_logger
	    // Output:
		// ----------------------------------------------------------------------
		//"level": "info",
		//"timestamp": "2022-03-17T07:57:46.069059Z",
		// An informational severity message
		// ----------------------------------------------------------------------
		// keywords::format =
		// (
			// expr::stream << "\"level\": "
			// << "\"" << triv::severity << "\", " << std::endl
			// << "\"timestamp\": \""
			// << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%dT%H:%M:%S.%fZ")// "%Y-%m-%d_%H:%M:%S.%f") 
			// << "\", "<< std::endl
			// << expr::smessage
		// ),
			
        boost::log::keywords::auto_flush = true
    );

    // Output message to file, rotates when file reached 1mb or at midnight every day. Each log file
    // is capped at 1mb and total is 20mb
    boost::log::add_file_log (
        // boost::log::keywords::file_name = "sample_%3N.log",
        // boost::log::keywords::rotation_size = 1 * 1024 * 1024,
        // boost::log::keywords::max_size = 20 * 1024 * 1024,
        // boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        // boost::log::keywords::format = COMMON_FMT,
        // boost::log::keywords::auto_flush = true
		
		keywords::file_name = "sample.log",//"sample_%N.log",  /*< file name pattern >*/
		keywords::rotation_size = 10*1024*1204,                                 /*< rotate files every 10 MiB... >*/
		//keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
		
		// Fuerzo el mismo formato que a fichero!. Elegir en ambos add_console_log() y add_file_log() un mismo keywords::format sea el que sea:
		
		// Opcion 1 Basica
	    // Output:
		// ----------------------------------------------------------------------
		// [2022-03-17 08:08:32.277274][info]:  An informational severity message
		// ----------------------------------------------------------------------
		keywords::format = COMMON_FMT,
		
		// Opcion 2 inpa_logger
	    // Output:
		// ----------------------------------------------------------------------
		//"level": "info",
		//"timestamp": "2022-03-17T07:57:46.069059Z",
		// An informational severity message
		// ----------------------------------------------------------------------	
		// keywords::format =
		// (
			// expr::stream << "\"level\": "
			// << "\"" << triv::severity << "\", " << std::endl
			// << "\"timestamp\": \""
			// << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%dT%H:%M:%S.%fZ")// "%Y-%m-%d_%H:%M:%S.%f") 
			// << "\", "<< std::endl
			// << expr::smessage
		// ),
		
		
		keywords::auto_flush = true // Sin esto no guardaba en el fichero!! (OJO que el nuevo logger de apm no lo tiene aun ...)		
		
		
    );
	
	

    boost::log::add_common_attributes();

    // Only output message with INFO or higher severity in Release
#ifndef _DEBUG
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );
#endif

}

int main(int, char**)
{
    init_log();

    // Output some simple log message
	// Notar que no saca solo el  << msg (expr::smessage), saca tambien antes lo que hayamos metido en  keywords::format a expr::stream << antes de dicho expr::smessage. P.e. el level y el timestamp en la Opcion 2 inpa_logger
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return EXIT_SUCCESS;
}


