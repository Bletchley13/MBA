#include "qemu-common.h"
#include "monitor/monitor.h"
#include "net/slirp.h"

#include "agent-commands.h"
#include "agent.h"

/*
void do_win_impo(Monitor *mon, const QDict *qdict)
{
    if ( agent_thread == 1 ) {
        char *srcpath = (char *)qdict_get_str(qdict, "srcpath");
        char *despath = (char *)qdict_get_str(qdict, "despath");
        strcpy(agent_srcpath, srcpath);
        strcpy(agent_despath, despath);
        agent_action = 1;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
void do_win_expo(Monitor *mon, const QDict *qdict)
{
    if ( agent_thread == 1 ) {
	    char *srcpath = (char *)qdict_get_str(qdict, "srcpath");
	    char *despath = (char *)qdict_get_str(qdict, "despath");
        strcpy(agent_srcpath, srcpath);
        strcpy(agent_despath, despath);
        agent_action = 2;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
void do_win_exec(Monitor *mon, const QDict *qdict)
{
    if ( agent_thread == 1 ) {
	    char *despath = (char *)qdict_get_str(qdict, "despath");
        strcpy(agent_despath, despath);
        agent_action = 3;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
void do_win_invo(Monitor *mon, const QDict *qdict)
{
    if ( agent_thread == 1 ) {
	    char *despath = (char *)qdict_get_str(qdict, "despath");
        strcpy(agent_despath, despath);
	    agent_action = 4;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
void do_win_status(Monitor *mon, const QDict *qdict)
{
    if ( agent_thread == 1 ) {
        agent_action = 5;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
void do_win_logf(Monitor *mon, const QDict *qdict)
{
     if ( agent_thread == 1 ) {
	    char *despath = (char *)qdict_get_str(qdict, "despath");
        strcpy(agent_despath, despath);
        agent_action = 6;
    } // if
    else {
        monitor_printf(mon, "Please enter w_init to start thread\n");
    } // else
}
*/

void do_win_init(Monitor *mon, const QDict *qdict)
{
    MBA_AGENT_RETURN ret;

    // counter (down) for port forwarding setup try
    int try_countdown = 10; 

    uint16_t fwd_port;
    char     fwd_config[32];
        
    // check if the agent extension has been initialized
    if( agent_is_ready() ) { 
        monitor_printf( mon, "The agent has been initialized\n" );
        return;
    }   

    // setup port forwarding for in-VM agent server, 10 times trying with random port
    srand( time(NULL) );
    while( try_countdown ) { 

        fwd_port  = rand() % 65535;
        fwd_port += (fwd_port < 1024 )? 1024 : 0;

        bzero( fwd_config, sizeof(fwd_config) );
        snprintf( fwd_config, 32, "tcp:%d::%d", fwd_port, AGENT_GUEST_PORT );

        if( net_slirp_redir(fwd_config) == 0 ) 
            break;

        --try_countdown;
    }   

    if( try_countdown == 0 ) { 
        monitor_printf( mon, "Agent port forwarding setup failed\n" );
        return;
    }   

    // initialize agent
    ret = agent_init( mon, fwd_port );
    switch( ret ) { 

        case AGENT_RET_SUCCESS:
            monitor_printf( mon, "Agent thread starting\n" );
            break;

        case AGENT_RET_EBUSY:
            monitor_printf( mon, "Agent is busy handling the previous command. Come back later\n" );
            break;

        case AGENT_RET_EFAIL:
            monitor_printf( mon, "Agent thread start failed\n" );
            break;

        default:
            monitor_printf( mon, "Unkown error when initializing agent\n" );
            break;
    }   

}

