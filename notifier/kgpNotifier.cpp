#include "kgpNotifier.h"

#include <iostream>

using namespace Notify;


void help()
{
	std::cout
	<< "Usage: kgp-notifer [options]\n"
	<< "\t-a id:\t\tAck message with 'id'\n"
	<< "\t-b:\t\tFor a new message, set it not to be removed on reboot\n"
	<< "\t-i issuer:\tAdd 'issuer' to new message\n"
	<< "\t-l level:\tThe log level for the new message (LOG_NOTICE, LOG_WARNING as string)\n"
	<< "\t-m body:\tThe body of the message\n"
	<< "\t-p:\t\tSet the message to be persistant, meaning not autoremoved over time\n"
	<< "\t-q:\t\tTry to be really quiet\n"
	<< "\t-s:\t\tUse only at boot to trigger notifiers\n";
}

int main(int argc, char **argv)
{

	int c;
    bool clearonboot = true;
    bool persistant = false;
    bool startup = false;
    bool ack = false;
    bool quiet = false;

    string loglevel;
    string msgbody;
    string issuer = "";
    string id;

    if (argc == 1)
    {
		std::cout << "Missing arguments.\n";
        help();
        return 1;
    }
    while ((c = getopt (argc, argv, "a:bi:l:m:psq")) != -1)
    {
        switch (c)
        {
        case 'a': // ack message with id
            id = optarg;
            ack = true;
            break;
        case 'b': // set message not to be removed on boot
            clearonboot = false;
            break;
        case 'i':  // set issuer
            issuer = optarg;
            break;
        case 'l':  // log level of message
            loglevel = optarg;
            break;
        case 'm':  // message body
            msgbody = optarg;
            break;
        case 'p':  // make message persistant
            persistant = true;
            break;
        case 's':
            startup = true;
            break;
        case 'q':
            quiet = true;
            break;
        default:
            help();
            return 1;
        }
    }
    if( argc > optind )
    {
		std::cout << "Invalid use.\n";
        help();
        return 1;
    }

    // check mandatory args
    if( startup )
    {
        // this should be run at boot in order to trigger notifiers of existing messages
        Message msg;
        msg.CleanUp(true);
        msg.ResetNotifiers(LOG_DEBUG);

    }
    else if (ack)  // ack existing message
    {
        if( id.size() != 36 )
        {
			std::cout << "Invalid message id.\n";
            return 1;
        }
        else
        {
            int triggers;
            ExistingMessage msg(id);

            triggers = msg.Ack();
            if( triggers >= 0)
            {
                // triggers will be <0 if something is wrong...
                if ( ! quiet )
                {
					std::cout << triggers<<"\n";
                }
            }
            else
            {
                if ( ! quiet )
                {
					std::cout << "Failed to ack message";
                }
                return 1;
            }

        }
    }
    else
    {
        // create a new message
        NewMessage msg;
        if(ltoi(loglevel) < 0)
        {
			std::cout << "Invalid loglevel: " << ltoi(loglevel) <<"("<< loglevel<<")\n";
            return 1;
        }
        if(msgbody.size() == 0)
        {
			std::cout << "Can not create message with empty body\n";
            return 1;
        }

        msg.Details(loglevel,msgbody,issuer,persistant,clearonboot);
        id=msg.GetID();
        msg.Send();
        if ( ! quiet )
        {
			std::cout << id<<"\n";
        }

    }

    return 0;
}



