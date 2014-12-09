<?php
/**
	PHP File for parsing groups of data
**/
class ParseGroups extends ParseProcess{
	//process column names
	public function parseColumnNames($stmt,$index){
		//echo $stmt[$index]['token']." ";
		$table = $_SESSION['tables'];//get names of tables
		
		$p = new ParseProcess(); // instantiate a ParseProcess class

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			$tables = $_SESSION['tables'];//get names of current tables
			$table_names = explode(",",$tables);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": // open parenthesis
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //column name of a table
						// first column to be projected
						if($_SESSION['columns']=="")
							$_SESSION['columns'].=strtoupper($lexeme);
						// next column/s to be projected
						else $_SESSION['columns'].=",".strtoupper($lexeme);

						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="TABLE_SELECT_OPERATOR"){ // FROM statement
							$p = new ProcessSelect();
							$p->parseSelect($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME": //column name of a table
						// first column to be projected
						if($_SESSION['columns']=="")
							$_SESSION['columns'].=strtoupper($lexeme);
						// next column/s to be projected
						else $_SESSION['columns'].=",".strtoupper($lexeme);

						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="TABLE_SELECT_OPERATOR"){ // FROM statement
							$p = new ProcessSelect();
							$p->parseSelect($stmt,$index+1);
						}
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