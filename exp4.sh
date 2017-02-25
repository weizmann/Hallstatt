export PS4='+{$LINENO:${FUNCNAME[0]}}'

DEBUG()
{
	if [ "$DEBUG" =  "true" ]; then
		$@
	fi
}

DEBUG=true

a=1
DEBUG echo "a =  $a"

#DEBUG=false
DEBUG echo "a =  $a"


