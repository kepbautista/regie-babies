<?php
/**
	PHP File for parsing Select Statements
**/
class ProcessSelect extends ParseProcess{
	public $columns = "";//list all columns of the specified tables

	// get all columns of specified tables
	public function getColumnNames($table){
		//traverse each column of the table
		foreach($this->table_columns[$table] as $col){
			// first column read
			if($this->columns=="") $this->columns.=$col;
			// next column/s read
			else $this->columns.=",".$col;
		}
	}

	// get all table names in the SELECT statement
	public function getTableNames($stmt){
		$table_names = "";

		foreach($stmt as $value){
			if($value['token']=="TABLE_NAME"){
				// get all columns of the table
				$this->getColumnNames(strtoupper($value['lexeme']));

				// it is the first table name
				if($table_names=="") $table_names.=strtoupper($value['lexeme']);
				// next table names
				else $table_names.=",".strtoupper($value['lexeme']);
			}
		}

		return $table_names;
	}

	//parse start of SELECT Statement
	public function parseSelect($stmt,$index){
		//echo $stmt[$index]['token']." ";

		$p = new ProcessWhereStmt();//instantiate a ProcessWhereStmt class
		$pg = new ParseGroups();//instantiate a ParseGroups class
		$pj = new ProcessJoin();//instantiate a ProcessJoin class

		// get names of the tables in the SELECT statement
		if($index==0) $_SESSION['tables'] = $this->getTableNames($stmt);
		
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "PROJECT_COMMAND": // SELECT command
						if(in_array($nextTok, array("ASTERISK_CHARACTER",
							"COLUMN_NAME","NUMERIC_COLUMN_NAME")))
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ASTERISK_CHARACTER": // SELECT ALL character
						$_SESSION['columns'] = $this->columns;
						if($nextTok=="TABLE_SELECT_OPERATOR")
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": // non-numeric column name
						$pg->parseColumnNames($stmt,$index);
						break;
				case "NUMERIC_COLUMN_NAME": // numeric column name
						$pg->parseColumnNames($stmt,$index);
						break;
				case "TABLE_SELECT_OPERATOR": // FROM statement
						if($nextTok=="TABLE_NAME"||$nextTok=="JOIN_OPERATOR")
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_NAME": // name of a TABLE
						if(in_array($nextTok, array("VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR",
							"JOIN_OPERATOR","JOIN_CONDITION_OPERATOR")))
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma "," character
						if($nextTok=="TABLE_NAME")
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "SELECT_OPERATOR": // WHERE clause
						$p->parseWhereStmt($stmt,$index);
						break;
				case "JOIN_OPERATOR": // JOIN statement
						if($nextTok=="TABLE_NAME")
							$this->parseSelect($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "JOIN_CONDITION_OPERATOR": // ON statement
						$pj-> parseJoinStmt($stmt,$index);
						break;
				case "END_OF_STATEMENT":
						break;
			}
		}
	}
}