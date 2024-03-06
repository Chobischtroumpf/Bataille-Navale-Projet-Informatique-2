# API Endpoints


## QueryGameState
- **Endpoint**: `/api/games/query`
- **Method**: `GET`
- **Description**: Queries the game state by session ID.
- **Parameters**:
  - `sessionid`: The session ID of the game.
- **Returns**: Game details(in NJSON format).

## GetGames
- **Endpoint**: `/api/games`
- **Method**: `GET`
- **Description**: Retrieves a list of game sessions.
- **Parameters**: None
- **Returns**: A list of session IDs.

## CreateGame
- **Endpoint**: `/api/games/create`
- **Method**: `POST`
- **Description**: Creates a new game session with the provided game details.
- **Parameters**: `gameDetails` (in NJSON format) containing the game configuration.
- **Returns**: The session ID of the created game.

## JoinGame
- **Endpoint**: `/api/games/join`
- **Method**: `GET`
- **Description**: Joins a game session using the session ID.
- **Parameters**: `sessionId` query parameter to specify the game session to join.
- **Returns**: Game details of the joined session.

## MakeMove
- **Endpoint**: `/api/games/move`
- **Method**: `POST`
- **Description**: Makes a move in the specified game session.
- **Parameters**:
  - `sessionId`: The session ID of the game.
  - `move`: The move details.
- **Returns**: A bool indicating success.

## Login
- **Endpoint**: `/api/login`
- **Method**: `POST`
- **Description**: Authenticates a user.
- **Parameters**:
  - `userId`: The user ID.
  - `password`: The user's password.
- **Returns**: A bool indicating success.


## Register
- **Endpoint**: `/api/register`
- **Method**: `POST`
- **Description**: Registers a user and authenticates them.
- **Parameters**:
  - `username`: The user's usernam.
  - `password`: The user's password.
- **Returns**: A bool indicating success.

## GetUserId
- **Endpoint**: `/api/login/uid`
- **Method**: `GET`
- **Description**: Retrieves the user ID using the username.
- **Parameters**: `username` query parameter.
- **Returns**: The user ID.

## SendMessage
- **Endpoint**: `/api/chat/send`
- **Method**: `POST`
- **Description**: Sends a message from one user to another.
- **Parameters**:
  - `recipientId`: The ID of the recipient.
  - `message`: The message content.
- **Returns**: A bool indicating success.

## GetMessages
- **Endpoint**: `/api/chat/get`
- **Method**: `GET`
- **Description**: Retrieves the conversation between the recipient and another user.
- **Parameters**: `recipientId` query parameter to specify the user whose conversations to retrieve.
- **Returns**: A conversation in JSON format.
