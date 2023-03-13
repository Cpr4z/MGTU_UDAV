#!/bin/sh

if [ -n "$1" ]

then 

cat << EOF > drivers.pro
TEMPLATE = subdirs

# You can turn building off for unneeded drivers,
# or turn building on for needed drivers
#
# Just add to list in this file subdirs with dirvers you need,
# or comment unneeded with # simbol

SUBDIRS += stub \\
    $1 
EOF

else

echo 
echo "No driver name passed"
echo "By default we turn on stub driver for testing purposes"
echo "You can create project file with driver you need by: ./drvon.sh [driver name]"

cat << EOF > drivers.pro
TEMPLATE = subdirs

# You can turn building off for unneeded drivers,
# or turn building on for needed drivers
#
# Just add to list in this file subdirs with dirvers you need,
# or comment unneeded with # simbol

SUBDIRS += stub

EOF



fi
