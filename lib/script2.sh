


if [ $# -gt 0 ]; then
    package=$1

    apt-cache depends --recurse --no-recommends --no-suggests --no-conflicts --no-breaks --no-replaces --no-enhances $package | grep "^\w" | sort -u > output.txt

    output=$(xargs apt list < output.txt | grep -v "installed" | grep -v "upgradable" | awk '{print $1}' | awk -F'/' '{print $1}' | sed 's/Listing...//')
    
    echo $output > output.txt
    xargs apt-get download  < output.txt

else
    echo "No argument provided."
fi
