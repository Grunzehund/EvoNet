#ifndef __NETWORK_DEFINES_H
#define __NETWORK_DEFINES_H

/* This is our protocol
 *
 * Control-Port: 49638  --> Client to Server
 * Data-Port:    53404  --> Server to Client
 * Protocol: tcp/ip
 *
 * |content length as uint64_t|_|command as uint16_t|_|data|
 *
 * commands:
 * -  0: connection refused , error msg                                                              [char [msg]]
 * -  1: error ,  error msg                                                                          [char [msg]]
 * -  2: get update of map / creatures                                                               [map width(uint32_t), map height(uint32_t), map data --> [lines(uint16_t)], creature count(uint64_t), [creatures (uint64_t id, double age, double energy, double x, double y, double rotation, uint8_t feeler count, [double feeler_rotation, double feeler_occlusion]]]
 * -  3: get file of whole simulation                                                                [char [filename]]
 * -  4: safe simulation to file, empty is default name, name is filename                            [char [filename]]
 * -  5: get simulation runtime as render steps                                                      [uint64_t steps]
 * -  6: set render speed as render steps per second, -1 means infinity, 0 means pause               [uint16_t render_speed]
 * -  7: view creatures brain                                                                        [uint64_t layer_count, [uint16_t neuron_per_layer_count][double neuron_value], [double connection_values]]
 * -  8: add creature                                                                                [double energy, double age, double x, double y, double rotation, uint8_t feeler count, [double feeler_rotation, double feeler_occlusion]]
 * -  9: delete creature                                                                             [uint64_t id]
 * - 10: set creature                                                                                [uint64_t id, double energy, double age, double x, double y, double rotation, uint8_t feeler count, [double feeler_rotation, double feeler_occlusion]]
 * - 11: set field value                                                                             [uint32_t x, uint32_t y, double var]
 * - 12: simulation shutdown                                                                         []
 * - 13: load old simulation file                                                                    [char [filename]]
 * - 14: list all available simulations                                                              [uint64_t file_found_count, [uint16_t filename length, char [filename]]]
 * - 15: get stats                                                                                   [not yet implemented]
 * - 16: set simulation rules                                                                        [double growth_speed, double creature_moving_speed, double creature_age_multiplier, double water_multiplier, double birth_energy, uint64_t max_creature_count, uint64_t min_creature_count]
 * - 17: Reset Stats                                                                                 []
 * - 18: Get Info                                                                                    [char [msg]]
 */

#define CONTROL_PORT 49638
#define DATA_PORT    53404

#define HEADER_SIZE 10

#define LENGTH_POS 0
#define COMMAND_POS 8
#define DATA_POS 10

#define CMD_CONNECTION_REFUSED         0
#define CMD_ERROR                      1
#define CMD_UPDATE_MAP                 2
#define CMD_GET_SIMULATION             3
#define CMD_GET_SIMULATION_FILE        4
#define CMD_GET_RUNTIME                5
#define CMD_SET_RENDER_SPEED           6
#define CMD_GET_CREATURES_BRAIN        7
#define CMD_ADD_CREATURE               8
#define CMD_DELETE_CREATURE            9
#define CMD_SET_CREATURE               10
#define CMD_SET_FIELD_VALUE            11
#define CMD_SIMULATION_SHUTDOWN        12
#define CMD_LOAD_SIMULATION_FILE       13
#define CMD_LIST_AVAILABLE_SIMULATIONS 14
#define CMD_GET_STATS                  15
#define CMD_SET_RULES                  16
#define CMD_RESET_STATS                17
#define CMD_GET_INFO                   18

#define ERROR_COMMAND_NOT_RECOGNIZED     0
#define ERROR_UNKNOWN                    1
#define ERROR_CONNECTION_TIMED_OUT       2
#define ERROR_MAX_CREATURE_COUNT_REACHED 3
#define ERROR_FILENAME_ALREADY_EXISTS    4
#define ERROR_ID_NOT_EXISTS              5
#define ERROR_SIMUATION_SPEED_IMPOSSIBLE 6

#endif //__NETWORK_DEFINES_H

/* other ideas
 * 
 */
