/*
  Autor : Equipe Embarcados
*/

// Incluindo biblioteca 
#include "painlessMesh.h"

#define   MESH_PREFIX     "FranzininhoMesh" // Definido um nome para a rede
#define   MESH_PASSWORD   "franz123" // Definido senha
#define   MESH_PORT       5555                // Porta TCP padrão

#define   BTN_GPIO 2 //Definindo botão

//Declarando os objetos 
Scheduler userScheduler; 
painlessMesh  mesh;

bool last_state = HIGH;
bool state;

void send_msg(); 

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &send_msg );

void send_msg() 
{
  String msg = "Franzininho WiFi";
  msg += mesh.getNodeId();
  state = digitalRead(BTN_GPIO);
  
  if(state != last_state && state == LOW)
  {
    Serial.println("Botão pressionado | MSG : " + msg);
    mesh.sendBroadcast(msg); 
  }
  
  last_state = state;
}


void received_cb( uint32_t from, String &msg ) 
{
  Serial.printf("Recebi mensagem - %s | ID - %u \n", msg.c_str(),from);
}

void new_con_cb(uint32_t nodeId) 
{
    Serial.printf("[Nova conexão] - ID = %u\n", nodeId);
}

void changed_con_cb() 
{
  Serial.printf("Conexão alterada\n");
}

void node_time_adjusted_cb(int32_t offset) 
{
  Serial.printf("Ajuste de tempo %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() 
{
  //Inicializa o canal de Comunicação Serial em 115200 bps
  Serial.begin(115200);
  
  pinMode(BTN_GPIO,INPUT_PULLUP);
  
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // Habilita todo tipo de mensagens
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // habilita as mensagens de debug antes do init
  
  // Inicializa a rede mesh
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
 
  // Registrando as funções de callback para cada tipo de ocorrência na rede 
  // -  Recepção de dados
  // -  Nova Conexão
  // -  Alterando Conexão e 
  // -  Ajuste de tempo nas mensagens
 
  mesh.onReceive(&received_cb);
  mesh.onNewConnection(&new_con_cb);
  mesh.onChangedConnections(&changed_con_cb);
  mesh.onNodeTimeAdjusted(&node_time_adjusted_cb);
  
  // Adiciona uma task responsável por enviar as mensagens  
  userScheduler.addTask(taskSendMessage);
  // Inicializa a task de envio de mensagens
  taskSendMessage.enable();
}

void loop() 
{
  // Mantém outras tasks do mesh em operação 
  mesh.update();
  
}
