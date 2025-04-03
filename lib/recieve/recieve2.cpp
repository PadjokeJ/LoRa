void recieve(){
  unsigned long startMillis = millis();  // Record the starting time
  while (millis() - startMillis < 2000) {  // Loop for 2 seconds (2000 milliseconds)
    if (rf95.available()) {
            byte receivedMessage[MAX_MESSAGE_SIZE];  // Buffer to store received data
            unsigned char len = sizeof(receivedMessage);

            if (rf95.recv(receivedMessage, &len)) {  // Receive packet
                // Store message length and message if space available
                if (messageQueue.isFull() && lengthQueue.isFull()) {
                    unsigned char messageLength;
                    byte dequeuedMessage[MAX_MESSAGE_SIZE];
                    lengthQueue.pop(&messageLength);  // Pop message length
                    messageQueue.pop(dequeuedMessage);  // Pop actual message
                    Serial.println("Queue full, dropping first message");
                }
                lengthQueue.push(&len);
                messageQueue.push(receivedMessage);
                Serial.println("Message stored in queue.");
            } else {
                Serial.println("Receive failed.");
            }
        }
  }
}
