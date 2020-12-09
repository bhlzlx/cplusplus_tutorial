AddExecutable( helloworld "Samples" )
AddExecutable( basics "Samples" )
AddExecutable( memory_management "Samples" )
AddExecutable( forward_declare "Samples" )
AddExecutable( linkage "Samples" )
AddExecutable( pointer_reference "Samples" )
AddExecutable( perfect_forwarding "Samples" )
AddExecutable( multi_threading "Samples" )
AddExecutable( pointer_cast "Samples" )
AddExecutable( entry_point "Samples" )
AddExecutable( polymorphism "Samples" )
AddExecutable( multiInher_polymorph "Samples" )
AddExecutable( member_offset "Samples" )
AddExecutable( lrucache "Samples" )
AddExecutable( bigInteger "Samples" )
AddExecutable( pmr "Samples" )


target_link_libraries(  linkage
    PUBLIC base64
)