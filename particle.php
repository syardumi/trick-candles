<?php
ini_set('display_errors',1);  error_reporting(E_ERROR);

//Create a separate php file for Particle login credentials. It should have just one line on which $creds is defined as a string in the format:
//
// "-d username=eeeeemmmaaaillll@bbbb.com -d password=xxxxxx"
//
include "creds.php";

//check that this script is run via POST only
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    echo '{ "ok": false, "msg": "Method was not POST" }';
    exit;
}

//get an access token ::: CURL is required on the host server
exec("curl https://api.particle.io/oauth/token -u particle:particle -d grant_type=password ".$creds, $results);

$access_token;
foreach($results as $result) {
	//check results for match on access_token and use it 
	if (stripos($result, "access_token") !== false)
	{
		$pieces = explode(":", $result);
		$access_token = str_replace('"', '', str_replace('",', '', trim($pieces[1])));
		break;
	}
}

//publish event to the Particle cloud w/ data
exec('curl https://api.particle.io/v1/devices/events -d "name='.$_POST['action'].'" -d "data='.$_POST['params'].'" -d "ttl=60" -d access_token='.$access_token, $eventResults);

foreach($eventResults as $er){
	echo $er;
}