void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
}
int ledState = 1;

void loop() {
  String locs;
  int x_loc;
  int y_loc;
  int chess_switch;
  int player = 1;
  int board[9] = {0};
  int gamestate = 0;
  unsigned long currentMillis;
  unsigned long previousMillis = 0;
  while(true){
    while(Serial.available() <= 0){
        currentMillis = millis();
        if (currentMillis - previousMillis > 500) {
            previousMillis = currentMillis;
            if (ledState == LOW) {
                ledState = HIGH;
            } 
            else {
                ledState = LOW;
            }
            for(int i = 0; i < 9; i ++){
                if(board[i] == 2){
                    digitalWrite(i + 2, ledState);
                }
            }
        }
    };
    if (Serial.available() > 0){
        locs = Serial.readStringUntil('\n');
        Serial.println(locs);
        x_loc = (int) locs[0] - 48;
        y_loc = (int) locs[2] - 48;
    }
    chess_switch = (x_loc - 1) * 3 + y_loc;
    Serial.println(chess_switch);
    if(chess_switch < 0 || chess_switch > 9 || board[chess_switch - 1] != 0){
        Serial.println("not inserted!");
        continue;
    }
    else{
        insert(player, chess_switch);
        if(player % 2 == 1){
            board[chess_switch - 1] = 1;
        }
        else{
            board[chess_switch - 1] = 2;
        }
        player = (player + 1) % 2;
    }
    gamestate = checkwinner(board);
    if(gamestate == 1){
        Serial.println("Player 1 win!");
        reset(board, &player);
    }
    else if(gamestate == 2){
        Serial.println("Player 2 win!");
        reset(board, &player);
    }
    else if (gamestate == 3){
        Serial.println("Stuck!");
        reset(board, &player);
    }
  }
}

void insert(int player, int chess_switch){
    if(player % 2 == 1){
        digitalWrite(chess_switch + 1,HIGH);
    }
    else{
        digitalWrite(chess_switch + 1,HIGH);
    }
    Serial.println("inserted!");
}

int checkwinner(int board[]){
    int product;
    int empty = 0;
    for(int i = 0; i < 3; i ++){
        product = board[i * 3] * board[i * 3 + 1] * board[i * 3 + 2];
        if (product == 1 || product == 8){
            return pow(product, 1/3);
        }
        if(empty == 0 && product == 0){
            empty = 1;
        }
    }
    for(int i = 0; i < 3; i ++){
        product = board[i] * board[i + 3] * board[i + 6];
        if (product == 1 || product == 8){
            return pow(product, 1/3);
        }
    }
    if (board[0] * board[4] * board[9] == 1 || board[2] * board[4] * board[6] == 1)
    {
        return 1;
    }
    if (board[0] * board[4] * board[9] == 8 || board[2] * board[4] * board[6] == 8)
    {
        return 2;
    }
    if(empty == 0){
        return 3;
    }
    return 0;
};

void reset(int board[], int *player){
    for(int i = 0; i < 9; i ++){
        board[i] = 0;
        digitalWrite(i + 2, LOW);
    }
    *player = 1;
}
