#ifdef __cplusplus
extern "C" {
#endif

	/* This tool implements a time driven event generator.
	 * Using this tool, events can be set to be invoked at future times.
	 * An event is raised by invoking a handler function defined as:
	 *     void (*handler)(void *args)
	 * This function defines a handler that takes a single argument. 
	 * However, this argument is generic enough to pass anything of interest.
	 * The granularity of the seting time delay is microseconds.
	 * This tool supports several out standing events.
	 * This tool guarantees in order event invocation even if events are set out of order.
	 * That is, set(10s, E1);set(3s, E2) would invoke E2 ahead of E1.
	 * 
	 * WARNING: This tool uses SIGALRM to implment the event timer. 
	 * No other handler for SIGALRM must be defined when using this tool.
	 * Instead, this tool provides the necessary functions of SIGALRM.
	 */ 


#include <sys/time.h>

	/* This function adds an event to the event queue.
	 * The handler is a function that is called when the event is raised.
	 * The args specifies the parameters to be passed to the handler function when it is called.
	 * The argLen specifies the size of the args.
	 * The value of argLen could be 0 when the value of args is NULL.
	 * The delay specifies after how much delay from present the event is raised.
	 * The delay is specified as a tupule <seconds, microseconds>.
	 * The delay cannot be <0, 0>.
	 * The event will be raised a short time after the specified delay.
	 * This implementation is not very accurate in keeping time.
	 * This function returns an -1 upon error and 0 upon success.
	 * This function generates the following errors. (errno is set upon error)
	 *     EINVAL -- invalid arguments to the function.
	 */
	int setEvent (struct timeval delay, void (*handler)(void *), void *args, int argLen);

	/* This function initializes the eventTimer program.
	 * This function must be called only once before any other eventTimer functions.
	 */ 
	void eventTimerInit ();

	/* This function terminates the eventTimer program.
	 * This function must be called only once at the end of the program.
	 * This function blocks, waiting for all the waiting events to be handled.
	 * This function could block for 2 to 3 seconds longer than necessary.
	 */ 
	void eventTimerQuit ();



#ifdef __cplusplus
}
#endif
