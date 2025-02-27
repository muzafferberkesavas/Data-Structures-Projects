#include "Network.h"
#include "fstream"
#include "iostream"
#include "sstream"
#include <vector>

Network::Network() {
}


void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    if (!commands.empty()){
        for (size_t index = 0; index < commands.size(); ++index) {
            string line = commands[index];
            size_t command_index = line.find(' ');
            string command = line.substr(0, command_index);
            string restOfTheLine = line.substr(command_index + 1);
            if (command == "MESSAGE") {
                time_t currentTime;
                time(&currentTime);
                string sender_id = message(restOfTheLine, 0);
                string receiver_id = message(restOfTheLine, 1);
                string fullmessage = message(restOfTheLine, 2);
                int frame_count;
                if (fullmessage.length() % message_limit == 0) {
                    frame_count = fullmessage.length() / message_limit;
                } else {
                    frame_count = fullmessage.length() / message_limit + 1;
                }
                Log log = Log(to_string(currentTime), fullmessage, frame_count, 0, sender_id, receiver_id, true,
                              ActivityType::MESSAGE_SENT);
                for (size_t i = 0; i < clients.size(); ++i) {
                    if (clients[i].client_id == sender_id) {
                        clients[i].log_entries.push_back(log);
                    }
                }
                cout << string(line.length() + 9, '-') << endl << "Command: " << line << endl
                     << string(line.length() + 9, '-') << endl << "Message to be sent: \"" << fullmessage << "\""
                     << endl << endl;
                if (fullmessage.length() > message_limit) {
                    int frame_number = 1;
                    if (fullmessage.length() % message_limit != 0) {
                        int cycleCount = fullmessage.length() / message_limit + 1;
                        for (int i = 0; i < cycleCount; ++i) {
                            if (fullmessage.length() < message_limit) {
                                string frame_message = fullmessage;
                                int sender_index = 0;
                                int receiver_index = 0;
                                for (size_t index = 0; index < clients.size(); ++index) {
                                    if (clients[index].client_id == sender_id) {
                                        sender_index = index;
                                    }
                                    if (clients[index].client_id == receiver_id) {
                                        receiver_index = index;
                                    }
                                }
                                Client sender = clients[sender_index];
                                if (sender.routing_table[receiver_id] == receiver_id) {
                                    Client receiver = clients[receiver_index];
                                    PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3,
                                                                                                       sender.client_mac,
                                                                                                       receiver.client_mac);
                                    NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                    receiver.client_ip);
                                    TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1,
                                                                                                          sender_port,
                                                                                                          receiver_port);
                                    ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                                sender.client_id,
                                                                                                                receiver.client_id,
                                                                                                                frame_message);
                                    stack<Packet *> stack;
                                    stack.push(applicationLayerPacket);
                                    stack.push(transportLayerPacket);
                                    stack.push(networkLayerPacket);
                                    stack.push(physicalLayerPacket);
                                    clients[sender_index].outgoing_queue.push(stack);
                                    cout << "Frame #" << frame_number << endl;
                                    physicalLayerPacket->print();
                                    networkLayerPacket->print();
                                    transportLayerPacket->print();
                                    applicationLayerPacket->print();
                                    cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                    cout << "Number of hops so far: " << log.number_of_hops << endl << "--------"
                                         << endl;
                                } else {
                                    int forwarder_index = 0;
                                    Client receiver = clients[receiver_index];
                                    for (size_t index2 = 0; index2 < clients.size(); ++index2) {
                                        if (clients[index2].client_id == sender.routing_table[receiver_id]) {
                                            forwarder_index = index2;
                                        }
                                    }
                                    Client forwarder = clients[forwarder_index];

                                    PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3,
                                                                                                       sender.client_mac,
                                                                                                       forwarder.client_mac);
                                    NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                    receiver.client_ip);
                                    TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1,
                                                                                                          sender_port,
                                                                                                          receiver_port);
                                    ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                                sender.client_id,
                                                                                                                receiver.client_id,
                                                                                                                frame_message);
                                    stack<Packet *> stack;
                                    stack.push(applicationLayerPacket);
                                    stack.push(transportLayerPacket);
                                    stack.push(networkLayerPacket);
                                    stack.push(physicalLayerPacket);
                                    clients[sender_index].outgoing_queue.push(stack);
                                    cout << "Frame #" << frame_number << endl;
                                    physicalLayerPacket->print();
                                    networkLayerPacket->print();
                                    transportLayerPacket->print();
                                    applicationLayerPacket->print();
                                    cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                    cout << "Number of hops so far: " << log.number_of_hops << endl << "--------"
                                         << endl;

                                }
                                fullmessage = "";
                            } else {
                                string frame_message = fullmessage.substr(0, message_limit);
                                int sender_index = 0;
                                int receiver_index = 0;
                                for (size_t index = 0; index < clients.size(); ++index) {
                                    if (clients[index].client_id == sender_id) {
                                        sender_index = index;
                                    }
                                    if (clients[index].client_id == receiver_id) {
                                        receiver_index = index;
                                    }
                                }
                                Client sender = clients[sender_index];
                                if (sender.routing_table[receiver_id] == receiver_id) {
                                    Client receiver = clients[receiver_index];
                                    PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3,
                                                                                                       sender.client_mac,
                                                                                                       receiver.client_mac);
                                    NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                    receiver.client_ip);
                                    TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1,
                                                                                                          sender_port,
                                                                                                          receiver_port);
                                    ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                                sender.client_id,
                                                                                                                receiver.client_id,
                                                                                                                frame_message);
                                    stack<Packet *> stack;
                                    stack.push(applicationLayerPacket);
                                    stack.push(transportLayerPacket);
                                    stack.push(networkLayerPacket);
                                    stack.push(physicalLayerPacket);
                                    clients[sender_index].outgoing_queue.push(stack);
                                    cout << "Frame #" << frame_number << endl;
                                    physicalLayerPacket->print();
                                    networkLayerPacket->print();
                                    transportLayerPacket->print();
                                    applicationLayerPacket->print();
                                    cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                    cout << "Number of hops so far: " << log.number_of_hops << endl << "--------"
                                         << endl;
                                } else {
                                    int forwarder_index = 0;
                                    Client receiver = clients[receiver_index];
                                    for (size_t index2 = 0; index2 < clients.size(); ++index2) {
                                        if (clients[index2].client_id == sender.routing_table[receiver_id]) {
                                            forwarder_index = index2;
                                        }
                                    }
                                    Client forwarder = clients[forwarder_index];

                                    PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3,
                                                                                                       sender.client_mac,
                                                                                                       forwarder.client_mac);
                                    NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                    receiver.client_ip);
                                    TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1,
                                                                                                          sender_port,
                                                                                                          receiver_port);
                                    ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                                sender.client_id,
                                                                                                                receiver.client_id,
                                                                                                                frame_message);
                                    stack<Packet *> stack;
                                    stack.push(applicationLayerPacket);
                                    stack.push(transportLayerPacket);
                                    stack.push(networkLayerPacket);
                                    stack.push(physicalLayerPacket);
                                    clients[sender_index].outgoing_queue.push(stack);
                                    cout << "Frame #" << frame_number << endl;
                                    physicalLayerPacket->print();
                                    networkLayerPacket->print();
                                    transportLayerPacket->print();
                                    applicationLayerPacket->print();
                                    cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                    cout << "Number of hops so far: " << log.number_of_hops << endl << "--------"
                                         << endl;

                                }
                                fullmessage = fullmessage.substr(message_limit);
                                frame_number++;
                            }
                        }
                    } else {
                        int cycleCount = fullmessage.length() / message_limit;
                        for (int i = 0; i < cycleCount; ++i) {
                            string frame_message = fullmessage.substr(0, message_limit);
                            int sender_index = 0;
                            int receiver_index = 0;
                            for (size_t index = 0; index < clients.size(); ++index) {
                                if (clients[index].client_id == sender_id) {
                                    sender_index = index;
                                }
                                if (clients[index].client_id == receiver_id) {
                                    receiver_index = index;
                                }
                            }
                            Client sender = clients[sender_index];
                            if (sender.routing_table[receiver_id] == receiver_id) {
                                Client receiver = clients[receiver_index];
                                PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3, sender.client_mac,
                                                                                                   receiver.client_mac);
                                NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                receiver.client_ip);
                                TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1, sender_port,
                                                                                                      receiver_port);
                                ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                            sender.client_id,
                                                                                                            receiver.client_id,
                                                                                                            frame_message);
                                stack<Packet *> stack;
                                stack.push(applicationLayerPacket);
                                stack.push(transportLayerPacket);
                                stack.push(networkLayerPacket);
                                stack.push(physicalLayerPacket);
                                clients[sender_index].outgoing_queue.push(stack);
                                cout << "Frame #" << frame_number << endl;
                                physicalLayerPacket->print();
                                networkLayerPacket->print();
                                transportLayerPacket->print();
                                applicationLayerPacket->print();
                                cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                cout << "Number of hops so far: " << log.number_of_hops << endl << "--------" << endl;
                            } else {
                                int forwarder_index = 0;
                                Client receiver = clients[receiver_index];
                                for (size_t index2 = 0; index2 < clients.size(); ++index2) {
                                    if (clients[index2].client_id == sender.routing_table[receiver_id]) {
                                        forwarder_index = index2;
                                    }
                                }
                                Client forwarder = clients[forwarder_index];

                                PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3, sender.client_mac,
                                                                                                   forwarder.client_mac);
                                NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                                receiver.client_ip);
                                TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1, sender_port,
                                                                                                      receiver_port);
                                ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0,
                                                                                                            sender.client_id,
                                                                                                            receiver.client_id,
                                                                                                            frame_message);
                                stack<Packet *> stack;
                                stack.push(applicationLayerPacket);
                                stack.push(transportLayerPacket);
                                stack.push(networkLayerPacket);
                                stack.push(physicalLayerPacket);
                                clients[sender_index].outgoing_queue.push(stack);
                                cout << "Frame #" << frame_number << endl;
                                physicalLayerPacket->print();
                                networkLayerPacket->print();
                                transportLayerPacket->print();
                                applicationLayerPacket->print();
                                cout << "Message chunk carried: \"" << frame_message << "\"" << endl;
                                cout << "Number of hops so far: " << log.number_of_hops << endl << "--------" << endl;

                            }
                            fullmessage = fullmessage.substr(message_limit);
                            frame_number++;
                        }
                    }
                } else {
                    int frame_number = 1;
                    int sender_index = 0;
                    int receiver_index = 0;
                    for (size_t index = 0; index < clients.size(); ++index) {
                        if (clients[index].client_id == sender_id) {
                            sender_index = index;
                        }
                        if (clients[index].client_id == receiver_id) {
                            receiver_index = index;
                        }
                    }
                    Client sender = clients[sender_index];
                    if (sender.routing_table[receiver_id] == receiver_id) {
                        Client receiver = clients[receiver_index];
                        PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3, sender.client_mac,
                                                                                           receiver.client_mac);
                        NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                        receiver.client_ip);
                        TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1, sender_port,
                                                                                              receiver_port);
                        ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0, sender.client_id,
                                                                                                    receiver.client_id,
                                                                                                    fullmessage);
                        stack<Packet *> stack;
                        stack.push(applicationLayerPacket);
                        stack.push(transportLayerPacket);
                        stack.push(networkLayerPacket);
                        stack.push(physicalLayerPacket);
                        clients[sender_index].outgoing_queue.push(stack);
                        cout << "Frame #" << frame_number << endl;
                        physicalLayerPacket->print();
                        networkLayerPacket->print();
                        transportLayerPacket->print();
                        applicationLayerPacket->print();
                        cout << "Message chunk carried: \"" << fullmessage << "\"" << endl;
                        cout << "Number of hops so far: " << log.number_of_hops << endl << "--------" << endl;
                    } else {
                        int forwarder_index = 0;
                        Client receiver = clients[receiver_index];
                        for (size_t index2 = 0; index2 < clients.size(); ++index2) {
                            if (clients[index2].client_id == sender.routing_table[receiver_id]) {
                                forwarder_index = index2;
                            }
                        }
                        Client forwarder = clients[forwarder_index];

                        PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(3, sender.client_mac,
                                                                                           forwarder.client_mac);
                        NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(2, sender.client_ip,
                                                                                        receiver.client_ip);
                        TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(1, sender_port,
                                                                                              receiver_port);
                        ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(0, sender.client_id,
                                                                                                    receiver.client_id,
                                                                                                    fullmessage);
                        stack<Packet *> stack;
                        stack.push(applicationLayerPacket);
                        stack.push(transportLayerPacket);
                        stack.push(networkLayerPacket);
                        stack.push(physicalLayerPacket);
                        clients[sender_index].outgoing_queue.push(stack);
                        cout << "Frame #" << frame_number << endl;
                        physicalLayerPacket->print();
                        networkLayerPacket->print();
                        transportLayerPacket->print();
                        applicationLayerPacket->print();
                        cout << "Message chunk carried: \"" << fullmessage << "\"" << endl;
                        cout << "Number of hops so far: " << log.number_of_hops << endl << "--------" << endl;
                    }
                }
            } else if (command == "SHOW_FRAME_INFO") {
                cout << string(line.length() + 9, '-') << endl << "Command: " << line << endl
                     << string(line.length() + 9, '-') << endl;
                string sender_id = message(restOfTheLine, 0);
                string queue_info = message(restOfTheLine, 1);
                string frame_number = restOfTheLine.substr(restOfTheLine.length() - 1);
                int client_index;
                bool isValid = true;
                for (int index = 0; index < clients.size(); ++index) {
                    if (clients[index].client_id == sender_id) {
                        client_index = index;
                        if (queue_info == "out") {
                            if (clients[index].outgoing_queue.empty()) {
                                isValid = false;
                            }
                            if (stoi(frame_number) > clients[index].outgoing_queue.size()) {
                                isValid = false;
                            }
                        } else {
                            if (clients[index].incoming_queue.empty()) {
                                isValid = false;
                            }
                            if (stoi(frame_number) > clients[index].incoming_queue.size()) {
                                isValid = false;
                            }
                        }
                    }
                }
                if (isValid) {
                    cout << "Current Frame #" << frame_number << " on the "
                         << (queue_info == "out" ? "outgoing" : "incoming") << " queue of client " << sender_id << endl;
                    Client client = clients[client_index];
                    if (queue_info == "out") {
                        queue<stack<Packet *>> tempQueue = client.outgoing_queue;
                        for (int i = 1; i < stoi(frame_number); ++i) {
                            tempQueue.pop();
                        }
                        stack<Packet *> tempStack = tempQueue.front();
                        PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        TransportLayerPacket *transportLayerPacket = dynamic_cast<TransportLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        int total_frame_size = 0;
                        int log_index = 0;
                        for (size_t i = 0; i < client.log_entries.size(); ++i) {
                            total_frame_size += client.log_entries[i].number_of_frames;
                            if (total_frame_size >= stoi(frame_number)) {
                                log_index = i;
                            }
                        }
                        cout << "Carried Message: \"" << applicationLayerPacket->message_data << "\"" << endl;
                        cout << "Layer 0 info: ";
                        applicationLayerPacket->print();
                        cout << "Layer 1 info: ";
                        transportLayerPacket->print();
                        cout << "Layer 2 info: ";
                        networkLayerPacket->print();
                        cout << "Layer 3 info: ";
                        physicalLayerPacket->print();
                        cout << "Number of hops so far: " << client.log_entries[log_index].number_of_hops << endl;
                    } else {

                        queue<stack<Packet *>> tempQueue = client.incoming_queue;
                        for (int i = 1; i < stoi(frame_number); ++i) {
                            tempQueue.pop();
                        }
                        stack<Packet *> tempStack = tempQueue.front();
                        PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        TransportLayerPacket *transportLayerPacket = dynamic_cast<TransportLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(tempStack.top());
                        tempStack.pop();
                        int total_frame_size = 0;
                        int log_index = 0;
                        for (size_t i = 0; i < client.log_entries.size(); ++i) {
                            total_frame_size += client.log_entries[i].number_of_frames;
                            if (total_frame_size >= stoi(frame_number)) {
                                log_index = i;
                            }
                        }
                        cout << "Carried Message: \"" << applicationLayerPacket->message_data << "\"" << endl;
                        cout << "Layer 0 info: ";
                        applicationLayerPacket->print();
                        cout << "Layer 1 info: ";
                        transportLayerPacket->print();
                        cout << "Layer 2 info: ";
                        networkLayerPacket->print();
                        cout << "Layer 3 info: ";
                        physicalLayerPacket->print();
                        cout << "Number of hops so far: 1" << endl;
                    }

                } else {
                    cout << "No such frame." << endl;
                }
            } else if (command == "SHOW_Q_INFO") {
                cout << string(line.length() + 9, '-') << endl << "Command: " << line << endl
                     << string(line.length() + 9, '-') << endl;
                string clientid = message(restOfTheLine, 0);
                string queue_info = message(restOfTheLine, 1);
                int total_frame_number = 0;
                if (queue_info == "out") {
                    int client_index;
                    for (int i = 0; i < clients.size(); ++i) {
                        if (clients[i].client_id == clientid) {
                            client_index = i;
                        }
                    }
                    total_frame_number = clients[client_index].outgoing_queue.size();
                    cout << "Client " << clientid << " Outgoing Queue Status" << endl;
                    cout << "Current total number of frames: " << total_frame_number << endl;
                } else {
                    int client_index;
                    for (int i = 0; i < clients.size(); ++i) {
                        if (clients[i].client_id == clientid) {
                            client_index = i;
                        }
                    }
                    total_frame_number = clients[client_index].incoming_queue.size();
                    cout << "Client " << clientid << " Incoming Queue Status" << endl;
                    cout << "Current total number of frames: " << total_frame_number << endl;
                }
            } else if (command == "SEND") {
                cout << string(line.length() + 9, '-') << endl << "Command: " << line << endl
                     << string(line.length() + 9, '-') << endl;
                for (size_t i = 0; i < clients.size(); ++i) {
                    if (!clients[i].outgoing_queue.empty()) {
                        int total_frame_size = 0;
                        int frame_number = 1;
                        int log_index;
                        int cycleCount = clients[i].outgoing_queue.size();
                        for (int j = 0; j < cycleCount; ++j) {
                            for (int index = 0; index < clients[i].log_entries.size(); ++index) {
                                total_frame_size += clients[i].log_entries[index].number_of_frames;
                                if (total_frame_size >= frame_number) {
                                    log_index = index;
                                    break;
                                } else {
                                    frame_number = 1;
                                }
                            }
                            PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(clients[i].outgoing_queue.front().top());
                            clients[i].outgoing_queue.front().pop();
                            NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(clients[i].outgoing_queue.front().top());
                            clients[i].outgoing_queue.front().pop();
                            TransportLayerPacket *transportLayerPacket = dynamic_cast<TransportLayerPacket *>(clients[i].outgoing_queue.front().top());
                            clients[i].outgoing_queue.front().pop();
                            ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(clients[i].outgoing_queue.front().top());
                            clients[i].outgoing_queue.front().pop();
                            string receiver_mac = physicalLayerPacket->receiver_MAC_address;
                            int receiver_index;
                            for (int k = 0; k < clients.size(); ++k) {
                                if (clients[k].client_mac == receiver_mac) {
                                    receiver_index = k;
                                }
                            }
                            stack<Packet *> stack;
                            stack.push(applicationLayerPacket);
                            stack.push(transportLayerPacket);
                            stack.push(networkLayerPacket);
                            stack.push(physicalLayerPacket);
                            clients[receiver_index].incoming_queue.push(stack);

                            cout << "Client " << clients[i].client_id << " sending frame #" << frame_number
                                 << " to client " << clients[receiver_index].client_id << endl;
                            physicalLayerPacket->print();
                            networkLayerPacket->print();
                            transportLayerPacket->print();
                            applicationLayerPacket->print();
                            cout << "Message chunk carried: \"" << applicationLayerPacket->message_data << "\"" << endl;
                            cout << "Number of hops so far: " << clients[i].log_entries[log_index].number_of_hops + 1
                                 << endl << "--------" << endl;
                            total_frame_size = 0;
                            frame_number++;
                            clients[i].outgoing_queue.pop();
                        }
                    }
                }
            } else if (command == "RECEIVE") {
                cout << string(line.length() + 9, '-') << endl << "Command: " << line << endl
                     << string(line.length() + 9, '-') << endl;

            for (size_t i = 0; i < clients.size(); ++i) {
                if (!clients[i].incoming_queue.empty()) {
                    int total_frame_size = 0;
                    int frame_number = 1;
                    int frame_number_2 = 1;
                    int sender_index = 0;
                    int log_index = -1;
                    int cycleCount = clients[i].incoming_queue.size();

                    for (int j = 0; j < cycleCount; ++j) {

                        PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(clients[i].incoming_queue.front().top());
                        clients[i].incoming_queue.front().pop();
                        NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(clients[i].incoming_queue.front().top());
                        clients[i].incoming_queue.front().pop();
                        TransportLayerPacket *transportLayerPacket = dynamic_cast<TransportLayerPacket *>(clients[i].incoming_queue.front().top());
                        clients[i].incoming_queue.front().pop();
                        ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(clients[i].incoming_queue.front().top());
                        clients[i].incoming_queue.front().pop();
                        clients[i].incoming_queue.pop();
                        for (int t = 0; t < clients.size(); ++t) {
                            if (clients[t].client_mac == physicalLayerPacket->sender_MAC_address) {
                                sender_index = t;
                            }
                        }
                        bool isValid = false;
                        for (int client = 0; client < clients.size(); ++client) {
                            if (clients[client].client_id == clients[i].routing_table[applicationLayerPacket->receiver_ID]){
                                isValid = true;
                            }
                        }
                        for (int index = 0; index < clients[sender_index].log_entries.size(); ++index) {
                            if (clients[sender_index].log_entries[index].receiver_id == applicationLayerPacket->receiver_ID) {
                                total_frame_size += clients[sender_index].log_entries[index].number_of_frames;
                                if (total_frame_size > frame_number) {
                                    break;
                                } else if (total_frame_size == frame_number) {
                                    time_t currentTime;
                                    time(&currentTime);
                                    if (applicationLayerPacket->receiver_ID == clients[i].client_id) {
                                        Log log = Log(to_string(currentTime),
                                                      clients[sender_index].log_entries[index].message_content,
                                                      frame_number_2,
                                                      clients[sender_index].log_entries[index].number_of_hops + 1,
                                                      applicationLayerPacket->sender_ID,
                                                      applicationLayerPacket->receiver_ID,
                                                      true, ActivityType::MESSAGE_RECEIVED);
                                        clients[i].log_entries.push_back(log);
                                    } else {
                                        bool isValid = false;
                                        for (int client = 0; client < clients.size(); ++client) {
                                            if (clients[client].client_id == clients[i].routing_table[applicationLayerPacket->receiver_ID]){
                                                isValid = true;
                                            }
                                        }
                                        if (!isValid) {
                                            Log log = Log(to_string(currentTime),
                                                          clients[sender_index].log_entries[index].message_content,
                                                          frame_number_2,
                                                          clients[sender_index].log_entries[index].number_of_hops + 1,
                                                          applicationLayerPacket->sender_ID,
                                                          applicationLayerPacket->receiver_ID,
                                                          false, ActivityType::MESSAGE_DROPPED);
                                            clients[i].log_entries.push_back(log);
                                        } else {
                                            Log log = Log(to_string(currentTime),
                                                          clients[sender_index].log_entries[index].message_content,
                                                          frame_number_2,
                                                          clients[sender_index].log_entries[index].number_of_hops + 1,
                                                          applicationLayerPacket->sender_ID,
                                                          applicationLayerPacket->receiver_ID,
                                                          true, ActivityType::MESSAGE_FORWARDED);
                                            clients[i].log_entries.push_back(log);
                                        }
                                    }
                                    log_index++;
                                } else {
                                    frame_number_2 = 1;
                                }
                            }
                        }
                        int newreceiver_index = 0;
                        for (size_t k = 0; k < clients.size(); ++k) {
                            if (clients[k].client_id == clients[i].routing_table[applicationLayerPacket->receiver_ID]) {
                                newreceiver_index = k;
                            }
                        }
                        if (clients[i].routing_table[applicationLayerPacket->receiver_ID] != "X"){
                            if (clients[i].client_id != applicationLayerPacket->receiver_ID) {
                                physicalLayerPacket->sender_MAC_address = clients[i].client_mac;
                                physicalLayerPacket->receiver_MAC_address = clients[newreceiver_index].client_mac;
                            }
                            stack<Packet *> stack;
                            stack.push(applicationLayerPacket);
                            stack.push(transportLayerPacket);
                            stack.push(networkLayerPacket);
                            stack.push(physicalLayerPacket);
                            clients[i].outgoing_queue.push(stack);
                        }

                        if (log_index != -1 &&
                            clients[i].log_entries[log_index].activity_type == ActivityType::MESSAGE_FORWARDED) {
                            cout << "Client " << clients[i].client_id << " receiving a message from client "
                                 << clients[sender_index].client_id << ", but intended for client "
                                 << applicationLayerPacket->receiver_ID << ". Forwarding..." << endl;
                            for (int k = 1; k < frame_number_2 + 1; ++k) {
                                cout << "Frame #" << k << " MAC address change: New sender MAC "
                                     << clients[i].client_mac << ", new receiver MAC "
                                     << clients[newreceiver_index].client_mac << endl;
                            }
                            cout << "--------" << endl;
                        } else if (log_index != -1 &&
                                   clients[i].log_entries[log_index].activity_type == ActivityType::MESSAGE_DROPPED) {
                            for (int k = 1; k < frame_number_2 + 1; ++k) {
                                cout << "Client " << clients[i].client_id << " receiving frame #" << k
                                     << " from client " << clients[sender_index].client_id
                                     << ", but intended for client " << applicationLayerPacket->receiver_ID
                                     << ". Forwarding..." << endl;
                                cout << "Error: Unreachable destination. Packets are dropped after "
                                     << clients[i].log_entries[log_index].number_of_hops << " hops!" << endl;
                            }
                            cout << "--------" << endl;
                        }/* else if (log_index != -1 &&
                                   clients[i].log_entries[log_index].activity_type == ActivityType::MESSAGE_RECEIVED) {
                            for (int k = 1; k < frame_number_2 + 1; ++k) {
                                cout << "Client " << clients[i].client_id << " receiving frame #" << k
                                     << " from client " << clients[sender_index].client_id
                                     << ", originating from client " << applicationLayerPacket->sender_ID << endl;
                                physicalLayerPacket->print();
                                networkLayerPacket->print();
                                transportLayerPacket->print();
                                applicationLayerPacket->print();
                                string message;
                                queue<std::stack<Packet *>> tempQueue = clients[i].outgoing_queue;
                                if (frame_number == frame_number_2) {
                                    for (int l = 1; l < k; ++l) {
                                        tempQueue.pop();
                                    }
                                } else {
                                    for (int l = 1; l < frame_number; ++l) {
                                        tempQueue.pop();
                                    }
                                }

                                if (!tempQueue.empty()) {
                                    PhysicalLayerPacket *physicalLayerPacket2 = dynamic_cast<PhysicalLayerPacket *>(tempQueue.front().top());
                                    tempQueue.front().pop();
                                    NetworkLayerPacket *networkLayerPacket2 = dynamic_cast<NetworkLayerPacket *>(tempQueue.front().top());
                                    tempQueue.front().pop();
                                    TransportLayerPacket *transportLayerPacket2 = dynamic_cast<TransportLayerPacket *>(tempQueue.front().top());
                                    tempQueue.front().pop();
                                    ApplicationLayerPacket *applicationLayerPacket2 = dynamic_cast<ApplicationLayerPacket *>(tempQueue.front().top());
                                    tempQueue.front().pop();
                                    cout << "Message chunk carried: \"" << applicationLayerPacket2->message_data << "\""
                                         << endl;
                                    cout << "Number of hops so far: "
                                         << clients[i].log_entries[log_index].number_of_hops << endl;
                                    cout << "--------" << endl;
                                    tempQueue.pop();
                                }
                            }
                            cout << "Client " << clients[i].client_id << " received the message \""
                                 << clients[i].log_entries[log_index].message_content << "\" from client "
                                 << applicationLayerPacket->sender_ID << "." << endl;
                            cout << "--------" << endl;
                        }*/

                        total_frame_size = 0;
                        frame_number++;
                        frame_number_2++;
                    }
                }
            }
            }else if (command == "PRINT_LOG"){
                cout<<string(line.length()+9,'-')<<endl<<"Command: "<<line<<endl<<string(line.length()+9,'-')<<endl;
                string client_id = message(restOfTheLine,0);
                for (size_t i = 0; i < clients.size(); ++i) {
                    if  (client_id == clients[i].client_id){
                        if (!clients[i].log_entries.empty()){
                            cout<<"Client "<<clients[i].client_id<<" Logs:"<<endl;
                            for (int j = 1; j < clients[i].log_entries.size() + 1; ++j) {
                                string activity;
                                if (clients[i].log_entries[j-1].activity_type == ActivityType::MESSAGE_RECEIVED){
                                    activity = "Message Received";
                                }else if (clients[i].log_entries[j-1].activity_type == ActivityType::MESSAGE_DROPPED){
                                    activity = "Message Dropped";
                                }else if (clients[i].log_entries[j-1].activity_type == ActivityType::MESSAGE_FORWARDED){
                                    activity = "Message Forwarded";
                                }else if (clients[i].log_entries[j-1].activity_type == ActivityType::MESSAGE_SENT){
                                    activity = "Message Sent";
                                }
                                time_t timestamp{};
                                istringstream ss(clients[i].log_entries[j-1].timestamp);
                                ss >> timestamp;
                                tm *ltm = localtime(&timestamp);
                                string hour, min, sec, day, mnth;
                                if (ltm->tm_hour <= 9){
                                    hour = '0' + to_string(ltm->tm_hour);
                                }else{
                                    hour = to_string(ltm->tm_hour);
                                }
                                if (ltm->tm_min <= 9){
                                    min = '0' + to_string(ltm->tm_min);
                                }else{
                                    min = to_string(ltm->tm_min);
                                }
                                if (ltm->tm_sec <= 9){
                                    sec = '0' + to_string(ltm->tm_sec);
                                }else{
                                    sec = to_string(ltm->tm_sec);
                                }
                                if (ltm->tm_mday <= 9){
                                    day = '0' + to_string(ltm->tm_mday);
                                }else{
                                    day = to_string(ltm->tm_mday);
                                }
                                if (ltm->tm_mon + 1 <= 9){
                                    mnth = '0' + to_string(ltm->tm_mon + 1);
                                }else{
                                    mnth = to_string(ltm->tm_mon + 1);
                                }
                                cout<<"--------------"<<endl;
                                cout<<"Log Entry #"<<j<<":"<<endl;
                                cout<<"Activity: "<<activity<<endl;
                                cout<<"Timestamp: "<<ltm->tm_year + 1900<<"-"<<mnth<<"-"<<day<<" "<<hour<<":"<<min<<":"<<sec<<endl;
                                cout<<"Number of frames: "<<clients[i].log_entries[j-1].number_of_frames<<endl;
                                cout<<"Number of hops: "<<clients[i].log_entries[j-1].number_of_hops<<endl;
                                cout<<"Sender ID: "<<clients[i].log_entries[j-1].sender_id<<endl;
                                cout<<"Receiver ID: "<<clients[i].log_entries[j-1].receiver_id<<endl;
                                cout << "Success: " << (clients[i].log_entries[j - 1].success_status ? "Yes" : "No") << endl;
                                if (activity == "Message Received" || activity == "Message Sent"){
                                    cout<<"Message: "<<"\""<<clients[i].log_entries[j-1].message_content<<"\""<<endl;
                                }
                            }
                        }
                    }
                }

            }else {
                cout<<string(line.length()+9,'-')<<endl<<"Command: "<<line<<endl<<string(line.length()+9,'-')<<endl<<"Invalid command."<<endl;
            }
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    // TODO: Read clients from the given input file and return a vector of Client instances.
    vector<Client> clients;
    string line;
    int client_count;
    ifstream file(filename);
    if (file.is_open()){
        string firstLine;
        if (getline(file, firstLine)){
            client_count = stoi(firstLine);
        }
        while(getline(file,line) && client_count != 0){
            istringstream iss(line);
            vector<string> elements;
            string element;
            while (iss >> element){
                elements.push_back(element);
            }
            string client_id = elements[0];
            string client_ip = elements[1];
            string client_mac = elements[2];
            Client client = Client(client_id, client_ip, client_mac);
            clients.push_back(client);
            client_count--;
        }
        file.close();
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    string line;
    ifstream file(filename);
    if (file.is_open()){
        for (size_t index = 0; index < clients.size(); ++index) {
            while (getline(file,line)){
                if (line == "-"){
                    break;
                } else{
                 istringstream iss(line);
                 vector<string> elements;
                 string element;
                    while (iss >> element){
                        elements.push_back(element);
                    }
                    string receiver_ID = elements[0];
                    string nextHop_ID = elements[1];
                    clients[index].routing_table[receiver_ID] = nextHop_ID;
                }
            }
        }
        file.close();
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    // TODO: Read commands from the given input file and return them as a vector of strings.
    vector<string> commands;
    string line;
    int command_count;
    ifstream file(filename);
    if (file.is_open()){
        string firstLine;
        if (getline(file, firstLine)){
            command_count = stoi(firstLine);
        }
        while (getline(file,line) && command_count != 0){
            commands.push_back(line);
        }
        file.close();
    }
    return commands;
}

string Network::message(string restOfTheLine,int index) {
    if (index == 0){
     size_t sender_id_index = restOfTheLine.find(' ');
     string sender_id = restOfTheLine.substr(0,sender_id_index);
        return sender_id;
    }else if (index == 1){
        size_t sender_id_index = restOfTheLine.find(' ');
        restOfTheLine = restOfTheLine.substr(sender_id_index+1);
        size_t receiver_id_index = restOfTheLine.find(' ');
        string receiver_id = restOfTheLine.substr(0,receiver_id_index);
        return receiver_id;
    }else if (index == 2){
        size_t sender_id_index = restOfTheLine.find(' ');
        restOfTheLine = restOfTheLine.substr( sender_id_index+1);
        size_t receiver_id_index = restOfTheLine.find(' ');
        restOfTheLine = restOfTheLine.substr(receiver_id_index+1);
        string message = restOfTheLine.substr(1,restOfTheLine.length()-2);
        return message;
    }else{
        return "";
    }
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.

}
