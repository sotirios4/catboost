

IF (SANITIZER_TYPE != "undefined")  # XXX

RECURSE(
    R-package
    app
    idl
    libs
    private
    pytest
    tools
    docs
)

IF (NOT OPENSOURCE)
RECURSE(
    
)
ENDIF()

IF (HAVE_CUDA)
RECURSE(
    cuda
)
ENDIF()

ENDIF()
