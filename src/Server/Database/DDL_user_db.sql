
CREATE TABLE IF NOT EXISTS Users (
    id_user INTEGER PRIMARY KEY,
    username VARCHAR(100) NOT NULL UNIQUE,
    hash_pwd VARCHAR(200) NOT NULL,
    salt VARCHAR(32) NOT NULL,
    CHECK(username <> '') -- Check that the username is not empty
);


CREATE TABLE IF NOT EXISTS Relations (
    id_relation INTEGER PRIMARY KEY,
    id_user_r INT NOT NULL,
    id_friend INT NOT NULL,
    UNIQUE (id_user_r, id_friend),
    FOREIGN KEY (id_user_r) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
    FOREIGN KEY (id_friend) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
    CHECK (id_user_r <> id_friend) -- Check that user is not equal to friend
);


CREATE TABLE IF NOT EXISTS Messages (
    id_msg INTEGER PRIMARY KEY,
    sender VARCHAR(100) NOT NULL,
    receiver VARCHAR(100) NOT NULL,
    msg VARCHAR(200) NOT NULL,
    msg_date_time DATETIME NOT NULL,
    FOREIGN KEY (sender) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
	FOREIGN KEY (receiver) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
    CHECK (sender <> receiver), -- Check that sender is not equal to receiver
    CHECK(msg <> '') -- Check that the message is not empty
);


CREATE TABLE IF NOT EXISTS GameStates (
    id_game_state INTEGER PRIMARY KEY,
    id_player1 INTEGER NOT NULL,
    id_player2 INTEGER NOT NULL,
    id_session VARCHAR(100) NOT NULL,
    game_state VARCHAR(10000) NOT NULL,
    state_date_time DATETIME NOT NULL,
    FOREIGN KEY (id_player1) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
	FOREIGN KEY (id_player2) REFERENCES Users(id_user)
        ON DELETE CASCADE 
        ON UPDATE NO ACTION,
    CHECK (id_player1 <> id_player2),
    CHECK(game_state <> '')
);


-- Create a trigger to prevent updates on the id columns

CREATE TRIGGER IF NOT EXISTS prevent_user_id_update
BEFORE UPDATE OF id_user ON Users
BEGIN
    SELECT RAISE(ABORT, 'Cannot update id column');
END;

CREATE TRIGGER IF NOT EXISTS prevent_relation_id_update
BEFORE UPDATE OF id_relation ON Relations
BEGIN
    SELECT RAISE(ABORT, 'Cannot update id column');
END;

CREATE TRIGGER IF NOT EXISTS prevent_msg_id_update
BEFORE UPDATE OF id_msg ON Messages
BEGIN
    SELECT RAISE(ABORT, 'Cannot update id column');
END;
