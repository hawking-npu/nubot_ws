execute_process(COMMAND "/home/jensen/nubot_ws/build/2D_Plot_Tool/plot_tool/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/jensen/nubot_ws/build/2D_Plot_Tool/plot_tool/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
