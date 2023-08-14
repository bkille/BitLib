file( GLOB EXAMPLE_SOURCES src/*.cpp )
foreach( examplesourcefile ${EXAMPLE_SOURCES} )
    # Cut off the file extension and directory path
    get_filename_component( examplename ${examplesourcefile} NAME_WE )
    add_executable( ${examplename} ${examplesourcefile} )
    # Make sure YourLib is linked to each app
    #target_link_libraries( ${examplename} YourLib )
endforeach( examplesourcefile ${EXAMPLE_SOURCES} )
