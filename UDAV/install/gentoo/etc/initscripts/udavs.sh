# UDAV Server Init Script

start() {

        echo -n "UDAV server is starting... ";
        nohup /usr/local/bin/udavs > /dev/null 2>&1 &

        echo "done" 
}

stop() {
        echo -n "Stopping  UDAV server... " 
        killall -9 udavs

        echo "done" 
}

# See how we were called.
case "$1" in
        start)
                start
                ;;
        stop)
                stop
                ;;
        restart)
                stop
                start
                ;;
        *)
                echo $"Usage:  {start|stop|restart}" 
                exit
esac