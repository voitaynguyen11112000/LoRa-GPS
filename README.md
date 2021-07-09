# LoRa-GPS
Hệ thống hoặc động tốt trong bán kính 700m với MCU stm32f103c8t6, GPS BDS ATGM336H, LoRa sx1278, ESP 8266. Ở mạch gửi, GPS sẽ gửi thông tin vị trí về MCU qua giao tiếp UART, MCU gửi dữ liệu cho LoRa qua giao tiếp SPI và LoRa sẽ gửi tín hiệu tới LoRa ở mạch nhận. Ở mạch nhận, LoRa nhận tín hiệu từ mạch gửi truyền qua MCU theo giap tiếp SPI, MCU gửi dữ liệu nhận được cho ESP8266 qua giao tiếp UART hoặc MCU gửi dữ liệu cho điện thoại chạy android để làm gateway đưa dữ liệu lên server adafruit. Dữ liệu sẽ được cập nhật lên điện thoại nhận và hiển thị dưới dạng bản đồ về vị trí. Hệ thống có hiển thị dữ liệu về phần trăm pin ở mạch gửi\
Link github mạch phần cứng: https://github.com/voitaynguyen11112000/LoRa---GPS-circuit-in-Altium.git \
Contacts: voitaynguyen1234@gmail.com
