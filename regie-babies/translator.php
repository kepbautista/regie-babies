<?php
/**
	Translator Class for converting SQL statements
	to executable statements
**/
class Translator{
	//list of key attributes
	public $key_attributes=array("STUDNO","STUDENT.STUDNO","STUDENTHISTORY.STUDNO",
					  "CNO","COURSE.CNO","COURSEOFFERING.CNO","SEMESTER",
					  "ACADYEAR","SECTION","COURSEOFFERING.SEMESTER",
					  "COURSEOFFERING.ACADYEAR","COURSEOFFERING.SECTION",
					  "STUDCOURSE.STUDNO","STUDCOURSE.CNO","STUDCOURSE.SEMESTER",
					  "STUDCOURSE.ACADYEAR");

	// clean the string by removing the slashes \' and \"
	public function removeQuotes($str){
		$str = str_replace('\"', "", $str);
		$str = str_replace("\'", "", $str);
		$str = str_replace("\n", " ", $str);
		return $str;
	}

	//function for calling the Query Optimizer & Executing a statement
	public function callQueryOptimizer($cmd,$cols,$value){
		//call query optimizer here...
		echo "<br/>-----------------------------------------------------------<br/>";
		echo "command: ".$cmd."<br/>columns: ".$cols;
		echo "<br/>where: ".$_SESSION['select']."<br/>join on: ".$_SESSION['join_on'];
		echo "<br/>tables: ".$_SESSION['tables']."<br/>values: ".$value;

		$where = $_SESSION['select'];
		$join_on = $_SESSION['join_on'];
		$tables = $_SESSION['tables'];

		// clean the commands by removing quotes :)
		$cmd = $this->removeQuotes($cmd);
		$cols = $this->removeQuotes($cols);
		$where = $this->removeQuotes($where);
		$join_on = $this->removeQuotes($join_on);
		$tables = $this->removeQuotes($tables);
		$value = $this->removeQuotes($value);

		$exec_str = "./oursql \"{$cmd}\" \"{$cols}\" \"{$where}\" \"{$join_on}\" \"{$tables}\" \"{$value}\"";

		echo "<br/><br/>".$exec_str."<br/><br/>";
		exec($exec_str,$out1);
		print_r($out1);
	}

	//function for translating an INSERT or UPDATE statement
	public function translateSetValues($cmd){
		$cols="";$value="";//initialize values for columns and values
		$flag=$ctr=0;//$ctr for flag...
		$columns=$_SESSION['columns'];
		$values=$_SESSION['set_values'];
		$count=count($columns);
		$n_values=count($values);

		//validate column and set_values if they match
		for($ctr=0;$ctr<$count;$ctr++){
			if($ctr==$n_values) break;//empty value can be a null value
			// check if value corresponds to BOOLEAN data type (0 or 1)
			else if(($columns[$ctr]['token_type']=="BOOLEAN_TOKEN")&&
				!in_array($values[$ctr]['lexeme'], array("0","1"))){
				$flag = 1;
				break;
			}
			// value does not correspond to column type
			else if(($columns[$ctr]['token_type']!=$values[$ctr]['token_type'])
				&&($values[$ctr]['token_type']!="NULL_TOKEN")&&
				($columns[$ctr]['token_type']!="BOOLEAN_TOKEN")){
				$flag = 1;
				break;
			}
		
			//key attribute cannot be a null value
			else if(in_array($columns[$ctr]['lexeme'], $this->key_attributes)
			   &&($values[$ctr]['token_type']=="NULL_TOKEN")){
				$flag = 2;
				break;	
			}
		}

		//incompatible data types
		if($flag==1){
			//remove slashes
			$column = $this->removeQuotes($columns[$ctr]['lexeme']);
			$value = $this->removeQuotes($values[$ctr]['lexeme']);
			echo '<br/>Syntax error: Incompatible data type of "'.$column.'" and "'.$value.'".<br/>';
			$_SESSION['error']=1;
		}
		//a value for a key attribute is required
		else if($flag==2){
			$column = $this->removeQuotes($columns[$ctr]['lexeme']);
			echo '<br/>Syntax error: Key attribute '.$column.' cannot be a NULL value.';
			$_SESSION['error']=1;
		}
		//more number of values than number of columns
		else if($n_values>$count){
			echo '<br/>Syntax error: Too much arguments for values...';
			$_SESSION['error']=1;
		}
		else if($ctr<=$count){
			/*
				processing columns by concatenating them into one string
				separate each lexeme by a comma
			*/				
						
			$i=0;
			foreach($_SESSION['columns'] as $col){
				if($i==0) $cols.=$col['lexeme'];
				else $cols.=",".$col['lexeme'];
				$i++;
			}

			/*
				processing values by concatenating them into one string
				separate each lexeme by a comma
			*/
			$no_of_values=count($_SESSION['columns']);//number of values to be processed
			for($i=0;$i<$no_of_values;$i++){
				if($i==0) $value.=$values[$i]['lexeme'];
				else if($i>=count($_SESSION['set_values'])) $value.=",NULL";
				else $value.=",".$values[$i]['lexeme'];
			}
			$this->callQueryOptimizer($cmd,$cols,$value);
		}
	}

	//function for translating a DELETE statement
	public function translateDelete($cmd){	
		//call query optimizer
		$this->callQueryOptimizer($cmd,"","");
	}

	//function for translating a SELECT statement
	public function translateSelect($cmd){
		$columns = $_SESSION['columns'];

		//call query optimizer
		$this->callQueryOptimizer($cmd,$columns,"");
	}
}
?>