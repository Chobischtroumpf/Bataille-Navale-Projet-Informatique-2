# API Endpoints


## `GET` QueryGameState
- **Endpoint**: `/api/games/query`
- **Method**: `GET`
- **Description**: Queries the game state by session ID.
- **Parameters**:
  - `sessionid`: The session ID of the game.
- **Returns**: Game details(in NJSON format).

## `GET` GetGames
- **Endpoint**: `/api/games`
- **Method**: `GET`
- **Description**: Retrieves a list of game sessions.
- **Parameters**: None
- **Returns**: A list of session IDs.

## `POST` CreateGame
- **Endpoint**: `/api/games/create`
- **Method**: `POST`
- **Description**: Creates a new game session with the provided game details.
- **Header**:
  - `Authorization`: Bearer your token
- **Parameters**: 
   - `gameDetails` (in NJSON format) containing the game configuration.
- **Returns**: The session ID of the created game.

## `GET` JoinGame
- **Endpoint**: `/api/games/join`
- **Method**: `GET`
- **Description**: Joins a game session using the session ID.
- **Parameters**: `sessionId` query parameter to specify the game session to join.
- **Returns**: Game details of the joined session.

## `POST` MakeMove
- **Endpoint**: `/api/games/move`
- **Method**: `POST`
- **Description**: Makes a move in the specified game session.
- **Header**:
  - `Authorization`: Bearer your token
- **Parameters**:
  - `sessionId`: The session ID of the game.
  - `move`: The move details.
- **Returns**: A bool indicating success.
```sh
curl -X POST -H 'Authorization: Bearer $(token)' -H "Content-type: application/json" -d '{"sessionId": "id", "move": "move"}' 'http://localhost:8080/api/games/move'
```


## `POST` Login
- **Endpoint**: `/api/login`
- **Method**: `POST`
- **Description**: Authenticates a user.
- **Parameters**:
  - `username`: The user's username.
  - `password`: The user's password.
- **Returns**: A bool indicating success.
```sh
curl -X POST -H "Content-type: application/json" -d '{"username": "yourname", "password": "yourpassword"}' 'http://localhost:8080/api/login'
```


## `POST` Register
- **Endpoint**: `/api/register`
- **Method**: `POST`
- **Description**: Registers a user and authenticates them.
- **Parameters**:
  - `username`: The user's username.
  - `password`: The user's password.
- **Returns**: A bool indicating success.
```sh
curl -X POST -H "Content-type: application/json" -d '{"username": "yourname", "password": "yourpassword"}' 'http://localhost:8080/api/register'
```


## `GET` GetUserId
- **Endpoint**: `/api/login/uid`
- **Method**: `GET`
- **Description**: Retrieves the user ID using the username.
- **Parameters**: `username` query parameter.
- **Returns**: The user ID.
```sh
curl -X GET -H "Content-type: application/json" -d '{"username": "yourname"}' 'http://localhost:8080/api/login/uid'
```

## `GET` GetUsername
- **Endpoint**: `/api/username`
- **Method**: `GET`
- **Description**: Retrieves the username of a user from the userId
- **Parameters**: `userId` query parameter.
- **Returns**: The username.
```sh
curl -X GET -H "Content-type: application/json" -d '{"userId": "yourUserId"}' 'http://localhost:8080/api/username'
```

## `POST` SendMessage
- **Endpoint**: `/api/chat/send`
- **Method**: `POST`
- **Description**: Sends a message from one user to another.
- **Header**:
  - `Authorization`: Bearer your token
- **Parameters**:
  - `recipientId`: The ID of the recipient.
  - `message`: The message content.
- **Returns**: A bool indicating success.
```sh
curl -X POST -H 'Authorization: Bearer $(token)' -H "Content-type: application/json" -d '{"recipientId": "name", "message": "your message"}' 'http://localhost:8080/api/chat/send'
```

## `GET` GetMessages
- **Endpoint**: `/api/chat/get`
- **Method**: `GET`
- **Description**: Retrieves the conversation between the recipient and another user.
- **Header**:
  - `Authorization`: Bearer your token
- **Parameters**: 
  - `recipientId` query parameter to specify the user whose conversations to retrieve.
- **Returns**: A conversation in JSON format.
```sh
curl -X GET -H 'Authorization: Bearer $(token)' -H "Content-type: application/json" -d '{"recipientId": "name"}' 'http://localhost:8080/api/chat/get'
```