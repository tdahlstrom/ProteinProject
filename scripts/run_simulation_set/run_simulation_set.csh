#!/bin/csh -f
# author: Thomas Dahlstrom
# date: Dec. 13, 2016

#User defines iterations of run
set iterations = $1

#User defines initial temperature of run
set init_temp = $2

#User defines second temperature of run
set second_temp = $3

#User defines final temperature of run
set final_temp = $4

set A = "_A"
set B = "_B"
set C = "_C"

source `pwd`/folder_generator.csh $init_temp $iterations $A
source `pwd`/folder_gen_large_number.csh $second_temp $iterations $B 30000000
source `pwd`/folder_gen_large_number.csh $final_temp $iterations $C 180000000


@ tasks = $iterations
@ running_tasks = 0

while ( 0 < $tasks )
	if ( `eval ps -fu $USER | grep "mdrun" | grep -v "grep" | awk "{print $2}" | wc -l` < 2 ) then
		sleep 30
	
		if ( `eval ps -fu $USER | grep "mdrun" | grep -v "grep" | awk "{print $2}" | wc -l` < 2) then
			@ tasks--
	
			@ mark_num = $iterations - $tasks

			if ( $mark_num < 10 ) then
				set mark_num = "0$mark_num"
			endif

			(grompp -f `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_$init_temp"K"_$mark_num$A.mdp -c `pwd`/mdp_generator_files/4FAK.19198.pdb.gro -p `pwd`/mdp_generator_files/4FAK.19198.pdb.top -o `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_topol_$init_temp"K"_$mark_num$A.tpr -po `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_mdout_$init_temp"K"_$mark_num$A.out && mdrun -s `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_topol_$init_temp"K"_$mark_num$A.tpr -table `pwd`/mdp_generator_files/10_12_lookup.xvg -tablep `pwd`/mdp_generator_files/10_12_lookup.xvg -c `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_confout_$init_temp"K"_$mark_num$A.gro -e `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_ener_$init_temp"K"_$mark_num$A.edr -g `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_md_$init_temp"K"_$mark_num$A.log -cpo `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_state_$init_temp"K"_$mark_num$A.cpt -o `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_traj_$init_temp"K"_$mark_num$A.trr -x `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_traj_$init_temp"K"_$mark_num$A.xtc && grompp -f `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_$second_temp"K"_$mark_num$B.mdp -c `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_topol_$init_temp"K"_$mark_num$A.tpr -o `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_topol_$second_temp"K"_$mark_num$B.tpr -t `pwd`/4FAK_$init_temp"K"_$mark_num$A/4FAK_state_$init_temp"K"_$mark_num$A.cpt  -p `pwd`/mdp_generator_files/4FAK.19198.pdb.top -po `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_mdout_$second_temp"K"_$mark_num$B.out && mdrun -s `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_topol_$second_temp"K"_$mark_num$B.tpr -table `pwd`/mdp_generator_files/10_12_lookup.xvg -tablep `pwd`/mdp_generator_files/10_12_lookup.xvg -c `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_confout_$second_temp"K"_$mark_num$B.gro -e `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_ener_$second_temp"K"_$mark_num$B.edr -g `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_md_$second_temp"K"_$mark_num$B.log -cpo `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_state_$second_temp"K"_$mark_num$B.cpt -o `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_traj_$second_temp"K"_$mark_num$B.trr -x `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_traj_$second_temp"K"_$mark_num$B.xtc && grompp -f `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_$final_temp"K"_$mark_num$C.mdp -c `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_topol_$second_temp"K"_$mark_num$B.tpr -o `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_topol_$final_temp"K"_$mark_num$C.tpr -t `pwd`/4FAK_$second_temp"K"_$mark_num$B/4FAK_state_$second_temp"K"_$mark_num$B.cpt  -p `pwd`/mdp_generator_files/4FAK.19198.pdb.top -po `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_mdout_$final_temp"K"_$mark_num$C.out && mdrun -s `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_topol_$final_temp"K"_$mark_num$C.tpr -table `pwd`/mdp_generator_files/10_12_lookup.xvg -tablep `pwd`/mdp_generator_files/10_12_lookup.xvg -c `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_confout_$final_temp"K"_$mark_num$C.gro -e `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_ener_$final_temp"K"_$mark_num$C.edr -g `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_md_$final_temp"K"_$mark_num$C.log -cpo `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_state_$final_temp"K"_$mark_num$C.cpt -o `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_traj_$final_temp"K"_$mark_num$C.trr -x `pwd`/4FAK_$final_temp"K"_$mark_num$C/4FAK_traj_$final_temp"K"_$mark_num$C.xtc) &
		endif
	endif
end
