# API Documentation
This is all the endpoints used for the communication between the client & the server.

------------------------------------------------------------------------------------------

### Game Client

#### _**QueryGameState**_

<details>
 <summary><code>GET</code> <code><b>/api/games/query</b></code> <code>(Allows the client to query the current state of a game session)</code></summary>


##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | sessionId     |  required | string   | The unique identifier for the game session  |
> | userId     |  required | string   | The unique identifier for the user |


##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `application/json`        | `JSON representation of the game state.`                                 |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```JavaScript
>  GET /api/games/query?sessionid=exampleSessionId&userid=exampleUserId
> ```

</details>

------------------------------------------------------------------------------------------

#### _**GetGames**_

<details>
 <summary><code>GET</code> <code><b>/api/games</b></code> <code>(Retrieves information about available games sessions)</code></summary>


##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `String representation of available game sessions`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  GET /api/games
> ```

</details>

------------------------------------------------------------------------------------------

#### _**CreateGame**_

<details>
 <summary><code>POST</code> <code><b>/api/game/create</b></code> <code>(Creates a new game)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | gameDetails     |  required | object (JSON)  | Details of the game to be created  |

##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `String representing the session ID of the newly created game session`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  POST /api/games/create
>  {
>   "sessionID": "exampleValue",
>  }
> ```

</details>

------------------------------------------------------------------------------------------

#### _**JoinGame**_

<details>
 <summary><code>GET</code> <code><b>/api/game/join</b></code> <code>(Joins a game session)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | sessionId    |  required | string  | The unique identifier for the game session  |

##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `application/json`       | `JSON representation of the game details for the joined session`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  GET /api/games/join?sessionId=exampleId
> ```

</details>

-----------------------------------------------------------------------------------------

#### _**MakeMove**_

<details>
 <summary><code>POST</code> <code><b>/api/game/move</b></code> <code>(Makes a move in a game session)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | sessionId    |  required | string  | The unique identifier for the game session  |
> | move    |  required | string  | The attempted move for the game session  |

##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `Boolean indicating success of the move`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  POST /api/games/move
> {
>   "sessionId": "exampleSessionId",
>   "move": "exampleMove"
> }
> ```

</details>

-----------------------------------------------------------------------------------------

#### _**Login**_

<details>
 <summary><code>POST</code> <code><b>/api/login</b></code> <code>(Performs login)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | userId    |  required | string  | The unique identifier for the user  |
> | password    |  required | string  | The unique key to log into the user's account  |

##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `Authentication token as a string`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  POST /api/login
> {
>   "userId": "exampleUserId",
>   "password": "examplePassword"
> }
> ```

</details>

-----------------------------------------------------------------------------------------

#### _**GetUserId**_

<details>
 <summary><code>GET</code> <code><b>/api/login/uid</b></code> <code>(Gets a user ID)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | username    |  required | string  | Name associated to a user |


##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `The user ID associated to the username`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  GET /api/login/uid?username=exampleUsername
> ```

</details>

-----------------------------------------------------------------------------------------

#### _**SendMessage**_

<details>
 <summary><code>POST</code> <code><b>/api/chat/send</b></code> <code>(Sends a message)</code></summary>

##### Parameters

> | name      |  type     | data type               | description                                                           |
> |-----------|-----------|-------------------------|-----------------------------------------------------------------------|
> | senderId    |  required | string  | The unique identifier for the sender |
> | recipientId    |  required | string  | The unique identifier for the recipient|
> | message    |  required | string  | The message to be sent |


##### Responses

> | http code     | content-type                      | response                                                            |
> |---------------|-----------------------------------|---------------------------------------------------------------------|
> | `200`         | `text/plain;charset=UTF-8`       | `Boolean value indicating success of the send`                                |
> | `400`         | `application/json`                | `{"code":"400","message":"Bad Request"}`                            |

##### Example Request

> ```javascript
>  POST /api/chat/send
> {
>   "senderId": "exampleSenderId",
>   "recipientId": "exampleRecipientId",
>   "message": "Example message"
> }
> ```

</details>

-----------------------------------------------------------------------------------------