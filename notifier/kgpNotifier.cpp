#include "kgpNotifier.h"

using namespace Notify;


void help()
{
    printf("Usage: kgp-notifer [options]\n");
    printf("\t-a id:\t\tAck message with 'id'\n");
    printf("\t-b:\t\tFor a new message, set it not to be removed on reboot\n");
    printf("\t-i issuer:\tAdd 'issuer' to new message\n");
    printf("\t-l level:\tThe log level for the new message (LOG_NOTICE, LOG_WARNING as string)\n");
    printf("\t-m body:\tThe body of the message\n");
    printf("\t-p:\t\tSet the message to be persistant, meaning not autoremoved over time\n");
    printf("\t-q:\t\tTry to be really quiet\n");
    printf("\t-s:\t\tUse only at boot to trigger notifiers\n");
}

int main(int argc, char **argv)
{

    int c,index;
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
        printf("Missing arguments.\n");
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
        printf("Invalid use.\n");
        help();
        return 1;
    }

    // check mandatory args
    if( startup )
    {
        // this should be run at boot in order to trigger notifiers of existing messages
        int triggers;

        Message msg;
        msg.CleanUp(true);
        msg.ResetNotifiers(LOG_DEBUG);

    }
    else if (ack)  // ack existing message
    {
        if( id.size() != 36 )
        {
            printf("Invalid message id.\n");
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
                    printf("%d\n",triggers);
                }
            }
            else
            {
                if ( ! quiet )
                {
                    printf("Failed to ack message");
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
            printf("Invalid loglevel: %d (%s)\n",ltoi(loglevel), loglevel.c_str());
            return 1;
        }
        if(msgbody.size() == 0)
        {
            printf("Can not create message with empty body\n");
            return 1;
        }

        msg.Details(loglevel,msgbody,issuer,persistant,clearonboot);
        id=msg.GetID();
        msg.Send();
        if ( ! quiet )
        {
            printf("%s\n",id.c_str());
        }

    }

    return 0;
}



