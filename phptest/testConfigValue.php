<?php

printf("Testing failed include ...");
$testTheme = function_exists("getConfigValue") && getTheme("webapps","theme") ? getTheme("webapps","theme") : "dummy";
if ($testTheme != "dummy") printf("FAIL, theme should have been the default 'dummy'\n");
else printf("PASS\n");


//define("GETTHEME","/usr/share/opi-themes/theme.php");
define("CONFIGWRAPPER","../sysconfigWrapper.php");
if (is_file(CONFIGWRAPPER))	include CONFIGWRAPPER;	

printf("Testing plain text call ...  ");
$testTheme = function_exists("getConfigValue") && getConfigValue("webapps","theme") ? getConfigValue("webapps","theme") : "dummy";

if ($testTheme == "dummy") printf("FAIL, theme should probably not be the default 'dummy'\n");
else printf("PASS\n");


printf("Testing json call ...");
$theme_json = function_exists("getConfigValue") && getConfigValue("webapps","theme",true) ? getConfigValue("webapps","theme",true) : "dummy";
$testTheme = false;
if (is_string($theme_json)) {
	$testTheme = json_decode($theme_json)->webapps->theme;
}
if (! $testTheme || $testTheme == "dummy") printf("FAIL, theme should probably not be the default 'dummy'\n");
else printf("PASS\n");


