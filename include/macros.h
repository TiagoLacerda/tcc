#ifdef DEBUG
    #define DEBUG_ONLY(x) x
    #define DEBUG_ONLY_BLOCK(x) do { x } while (0)
#else
    #define DEBUG_ONLY(x)
    #define DEBUG_ONLY_BLOCK(x)
#endif
