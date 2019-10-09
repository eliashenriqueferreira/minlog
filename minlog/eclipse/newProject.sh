#!/bin/bash
###########################################################################################################################################################################################################################################################
##//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##//:20191009104019 #--# 10:40 quarta-feira, 9 de outubro de 2019 - Notepad Plugin Developed by Elias Henrique Ferreira.
##//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## This is a Project Replicator script.
## Usage: ./newProject.sh <NewProjectName>
## This will create a new project in the same node of this project and will rename in Eclipse files like .project and .cproject
###########################################################################################################################################################################################################################################################

# Actual father directory must be the same name of this actual project and actual project must be eclipse.
ProjectName=`basename ${PWD/eclipse}`
NewProjectName=$1

if [ "na"$NewProjectName"da" == "nada" ];
then
	echo "Usage: ./newProject.sh <NewProjectName>"
	exit
fi

if [ "$NewProjectName" == "$ProjectName" ];
then
	echo "New project Name must be different of actual project [$ProjectName]"
	exit
fi

cd ../../

cp -rp $ProjectName $NewProjectName

cd $NewProjectName/eclipse

tmp_file1=$(mktemp -p .)
mv .project $tmp_file1
sed s/$ProjectName/$NewProjectName/g < $tmp_file1 > .project

tmp_file2=$(mktemp -p .)
mv .cproject $tmp_file2
sed s/$ProjectName/$NewProjectName/g < $tmp_file2 > .cproject

diff $tmp_file1 .project >/dev/null  2>&1
if [ $? != 1 ];
then
	echo "There is something wrong because .project was not changed!"
	exit
fi

diff $tmp_file2 .cproject >/dev/null  2>&1
if [ $? != 1 ];
then
	echo "There is something wrong because .cproject was not changed!"
	exit
fi

rm -f $tmp_file1
rm -f $tmp_file2
