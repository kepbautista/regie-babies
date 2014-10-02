<?php
//classes
include("parser.php");

//instantiate classes
$parse = new Parser();//parser class

$query=addslashes($_POST['query']);
echo $query."\n";

/*add double quotes at the start & end of string
  for passing to command line arguments in the executable
  files
*/
$query=$parse->addDoubleQuotes($query);

exec("./oursql {$query}",$out1);//run C++ program
exec("java Echo {$query}",$out2);//run Java program

print_r($out1);
print_r($out2);
?>
