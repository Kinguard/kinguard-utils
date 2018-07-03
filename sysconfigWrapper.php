<?php
	/* This file is intended to be included in php configs to read parameters
	* from the configdb file (sysconfig.json).
	*
	*
    *  Example usage:  
    *	define("CONFIGWRAPPER","/usr/share/kinguard-utils/sysconfigWrapper.php");
	*   if (is_file(CONFIGWRAPPER))	include CONFIGWRAPPER;	
	*   $testTheme = function_exists("getConfigValue") && getConfigValue("webapps","theme") ? getConfigValue("webapps","theme") : "dummy";
	*/

	
	function getConfigValue($ns, $key, $json = false) {
		if ($json) {
			// return the value as json, can be good if the type is needed.
			$config = shell_exec("/usr/sbin/kgp-sysinfo -c $ns -k $key");
			if ($config) {
				return rtrim($config);
			}
		} else
			{
			$config = shell_exec("/usr/sbin/kgp-sysinfo -p -c $ns -k $key");
			if ($config) {
				return rtrim($config);
			}
		}
	}

