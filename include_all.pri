##### GDAL ####

!exists(include_gdal.pri) {
    message("GDAL will not be used")
} else {
    include(include_gdal.pri)
}

##### PCL ####

#if we must check if we can use pcl
!isEmpty(CHECK_CAN_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we want absolutely use pcl library
contains( COMPUTREE, ctlibpcl ) {
    # we set that the PCL check test must pass
    MUST_USE_PCL = 1
}

#if we want absolutely use pcl (the PCL check test must pass)
!isEmpty(MUST_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we must check if pcl can be used
!isEmpty(CHECK_PCL) {
    include(pcl_default_path.pri)

    exists(pcl_user_path.pri) {
        include(pcl_user_path.pri)
    }

    include(pcl_check.pri)

    #if PCL can be used
    isEmpty(USE_PCL_ERROR_MSG) {
        include(include_pcl_necessary.pri)
        warning("PCL found and it will be used in this plugin")
    }
}

#if we must absolutely use PCL
!isEmpty(MUST_USE_PCL) {
    !contains(DEFINES, USE_PCL) {
        for(a, USE_PCL_ERROR_MSG) {
            warning("Error when search PCL : $${a}")
        }
        error("PCL not found, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_PCL) {
        !contains(DEFINES, USE_PCL) {
            warning(This plugin can use PCL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_PCL_ERROR_MSG) {
                for(a, USE_PCL_ERROR_MSG) {
                    warning("Error when search PCL : $${a}")
                }
            }
        }
    }
}

##### OPENCV ####

contains(DEFINES, USE_OPENCV_DEFAULT) {
    !exists(include_opencv_default.pri) {
        message("OPENCV will not be used")
    } else {
        include(include_opencv_default.pri)
    }
} else {
    !exists(include_opencv.pri) {
        message("OPENCV will not be used")
    } else {
        include(include_opencv.pri)
    }
}

!exists(include_osg.pri) {
    !exists(include_osg_default.pri) {
        error("File include_osg.pri not found ! Run script \"convertInclude.bat\" to create this file automatically")
    } else {
        include(include_osg_default.pri)
    }
} else {
    include(include_osg.pri)
}

!contains( DEFINES, USE_PCL ) {
    isEmpty(PLUGIN_SHARED_DIR) {
        INCLUDEPATH += ./eigen
    } else {
        INCLUDEPATH += $${PLUGIN_SHARED_DIR}/eigen
    }
}
