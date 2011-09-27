USE WapServer3DB;

DROP TABLE IF EXISTS tbChessTableList;
CREATE TABLE tbChessTableList
(
  TableID INT UNSIGNED NOT NULL AUTO_INCREMENT,
  State TINYINT UNSIGNED NOT NULL DEFAULT '1',
  IDPlayer0 INT UNSIGNED NOT NULL DEFAULT '0',
  IDPlayer1 INT UNSIGNED NULL,
  password VARCHAR(50) NULL,
  Bet INT UNSIGNED NULL DEFAULT '0',
  Time2Step INT UNSIGNED NULL DEFAULT '600',
  Time2Game INT UNSIGNED NULL,
  MinRating INT UNSIGNED NULL,
  MaxRating INT UNSIGNED NULL,
  StepNum SMALLINT UNSIGNED NOT NULL DEFAULT '0',
  XPlayer INT UNSIGNED NOT NULL DEFAULT '0',
  CurPlayer INT UNSIGNED NULL,
  DrawState TINYINT UNSIGNED NOT NULL DEFAULT '0',
  PlayerStepTime INT UNSIGNED NULL,
  Player0GameTime INT UNSIGNED NULL DEFAULT '0',
  Player1GameTime INT UNSIGNED NULL DEFAULT '0',
  OpGameTime INT UNSIGNED NULL,
  FieldState BLOB NULL,
  GameLog TEXT NULL,
  PRIMARY KEY( TableID )
);

#DESCRIBE tbChessTableList;
SELECT * FROM tbChessTableList;



DROP TABLE IF EXISTS tbChessRating;
CREATE TABLE tbChessRating
(
  PlayerID INT UNSIGNED NOT NULL DEFAULT '0',
  Rating INT UNSIGNED NOT NULL DEFAULT '0',
  Available BOOL DEFAULT '0',
  LastGameResult INT UNSIGNED DEFAULT '0',
  PRIMARY KEY(PlayerID)
);

INSERT INTO tbChessRating (PlayerID, Rating, Available) VALUES (100, 1000, TRUE);

#DESCRIBE tbChessRating;
SELECT * FROM tbChessRating;
