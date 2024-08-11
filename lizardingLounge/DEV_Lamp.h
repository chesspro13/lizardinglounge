struct DEV_Lamp : Service::LightBulb {
  
  SpanCharacteristic *power;
  SpanCharacteristic *name;
  
  DEV_Lamp(int ledPin, char* givenName ) : Service::LightBulb(){

    power=new Characteristic::On();  
    name=new Characteristic::ConfiguredName(givenName);
        
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);
  }

  boolean update(){
    // digitalWrite(ledPin,power->getNewVal());

    if(power->getNewVal()){

        digitalWrite(ledPin,LOW);
      
    } else {
      digitalWrite(ledPin,HIGH);
  }
    return(true);// loop
  
  } 

  void loop(){
  
  }
  
};
      
//////////////////////////////////