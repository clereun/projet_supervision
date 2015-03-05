#!/bin/bash
# util_nom,util_uid,util_groupe

echo "$(awk -F: '{
	if ( $3 == 0 )
		print $1 "$" $3 "$" $4 "$"
	if ( $3 >= 1000 && $3 <= 2000 )
		print $1 "$" $3 "$" $4 "$"
	
}' /etc/passwd )"