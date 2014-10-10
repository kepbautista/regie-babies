<?php
//classes
include("parser.php");

//instantiate classes
$parse = new Parser();//parser class

$query=trim(addslashes($_POST['query']));

//check if matching symbols
$msg=$parse->checkMatchingSymbols($query);

//there is an error
if($msg!="")
	echo $msg;
else{

	//tokenize input
	$toks=$parse->tokenizeQuery($query);
	var_dump($toks);

	/**
		Part for passing arguments to executable files
	**/
	/*add double quotes at the start & end of string
  	for passing to command line arguments in the executable
  	files
	*/
	//$query=$parse->addDoubleQuotes($query);

	/*exec("./oursql {$query}",$out1);//run C++ program
	exec("java Echo {$query}",$out2);//run Java program

	print_r($out1);
	print_r($out2);*/

	/**
		call query optimizer here...
	**/

	/**
		call storage manager here...
	**/

}
?>
