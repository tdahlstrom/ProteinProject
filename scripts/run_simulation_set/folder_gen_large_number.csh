#!/bin/csh -f
set ref_t = $1
set iterations = $2
set step_of_set = $3
set nsteps = $4

set files_location = `pwd`/mdp_generator_files
#set template_mdpfile_location = $files_location/template_mdp.mdp
#set 

@ i = 1

while ( $i <= $iterations )
	set folder_prefix = "4FAK_$ref_t"

	#If iternation number is less than 10, inserts a 0 in front of the iteration number
	#This additional zero is a naming convention
	if ( $i < 10 ) then
		set folder_suffix = "K_0$i"
	else
		set folder_suffix = "K_$i"
	endif

	set full_folder_name = $folder_prefix$folder_suffix$step_of_set
	set destination_mdpfile_location = `pwd`/$full_folder_name/$full_folder_name.mdp
	set destination_folder_location = `pwd`/$full_folder_name

	mkdir $full_folder_name

	#Copies mdp file
	cp $files_location/template_mdp.mdp $destination_mdpfile_location

	#Creates a random number/seed to be used as the gen_seed parameter
	set gen_seed = `shuf -i 0-2147483647 -n 1`

	#Creates a random number of steps between 500000 - 1500000
	#set nsteps = 1000
	#set nsteps = 30000000
	#set nsteps = `shuf -i 500-1500 -n 1`

	#Inserts ref_t into the mdp file
	sed -i "s/REF_T_VALUE/$ref_t/" $destination_mdpfile_location

	#Inserts gen_seed into the mdp file
	sed -i "s/GEN_SEED_VALUE/$gen_seed/" $destination_mdpfile_location

	#Inserts nsteps into the mdp file
	sed -i "s/NSTEPS_VALUE/$nsteps/" $destination_mdpfile_location

	#Copies top, gro, and frame file
	#cp $files_location/[0-9]* $destination_folder_location
	
	@ i++
end
