const lobby = {
    id: 345935, // integer
    title: "gamer room", // integer
    status: "open", // enum string [open, in progress]
    max_size: 6, // integer
    game_length: 180, // integer, seconds

    // array of lobby_clients
    clients: [
        lobby_client,
    ],
};

const lobby_client = {
    id: 235, // integer
    username: "minidomo", // string
    team: "red", // enum string [red, blue]
    in_lobby: false, // boolean, indicate whether the player is in the lobby or in the game
};