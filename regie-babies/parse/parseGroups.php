<?php
/**
	PHP File for parsing groups of data
**/
class ParseGroups{
	//process column names
	public function parseColumnNames($stmt,$index){
		//echo $stmt[$index]['token']." ";
		
		$p = new ParseProcess(); // instantiate a ParseProcess class

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			$tables = $_SESSION['table'];//get names of current tables
			$table_names = explode($tables);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": // open parenthesis
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //column name of a table
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME": //column name of a table
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": //comma
						if($nextTok=="COLUMN_NAME"||$nextTok=="NUMERIC_COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL":// close parenthesis
						if($nextTok=="INSERT_VALUES"){
							$p = new ProcessInsert();
							$p->insertValues($stmt,$index+1);
						}
						break;
			}
		}
	}
}
?>