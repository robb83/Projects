#!/bin/bash

HOST=pop.gmail.com
PORT=995
USER=usernam1@gmail.com
PASS=password1
TIMEOUT=2
MESSAGE_ARRIVED=
MESSAGES=0

skipwelcome() {
	while read -t $TIMEOUT MSG <&${COPROC[0]}
	do
		[[ "$MSG" == "+OK"* ]] && return 0
	done
	
	return 1
}

sendreceiv() {
	echo $@ >&${COPROC[1]}
	read -t $TIMEOUT MSG <&${COPROC[0]}
	
	if [[ "$MSG" == "+OK"* ]]; then
		return 0
	else
		return 1
	fi
}

send() {
	echo $@ >&${COPROC[1]}
}

pop3_auth() {
	if sendreceiv "USER $USER"; then
		if sendreceiv "PASS $PASS"; then
			return 0
		fi
	fi
	
	return 1
}

pop3_stat() {
	if sendreceiv "STAT"; then
		MESSAGES=$(echo $MSG | awk '{print $2}')
		return 0
	fi
	
	return 1
}

pop3_quit() {
	send "QUIT"
	
	return 0
}

# make SSL connection, authentication, get message number
coproc openssl s_client -connect $HOST:$PORT

if skipwelcome && pop3_auth && pop3_stat; then
	echo "STAT: $MESSAGES"
	if [[ -n $MESSAGES ]]; then

		# process message with top
		for ((c=1; c<=$MESSAGES; c++))
		do
			if sendreceiv "TOP $c 1"; then

				MATCHED_SUBJECT=
				MATCHED_FROM=

				while read -t $TIMEOUT MSG <&${COPROC[0]}
				do
					# end of message
					[[ "$MSG" =~ ^\.\s* ]] && break 
					
					[[ "$MSG" =~ ^Subject:.*Hello\ World.* ]] && MATCHED_SUBJECT=1

					[[ "$MSG" =~ ^From:.*robb83@gmail\.com.* ]] && MATCHED_FROM=1

				done

				if [[ -n "$MATCHED_SUBJECT" && -n "$MATCHED_FROM" ]]; then
					MAIL_ARRIVED=1
					send "DELE $c"
					break
				fi
			else
				#something wrong
				break
			fi
		done
	fi
fi

[[ -n "$COPROC_PID" ]] && pop3_quit && kill "$COPROC_PID"

if [[ -n "$MAIL_ARRIVED" ]]; then
	echo "Do something"
fi

exit 0
