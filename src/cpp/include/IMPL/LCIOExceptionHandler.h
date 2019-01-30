#ifndef IMPL_LCIOEXCEPTIONHANDLER_H
#define IMPL_LCIOEXCEPTIONHANDLER_H 1


namespace IMPL{

/** Sets the default error handling for unexpected and uncaught exceptions.
 */
  class LCIOExceptionHandler {
  public:
    /**
     *  @brief  Setup the unexpected and terminate std handlers.
     *          This function is thread safe and can be called many time without 
     *          unexpected behaviour, sucha as exceptions, data race, etc ...
     */
    static void setupHandlers();
    
  private:
    /**
     *  @brief  The unexpected and terminate handler function
     */
    static void handler();
  };
} // namespace

#endif
