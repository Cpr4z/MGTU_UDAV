#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include <QCoreApplication>

namespace Core
{
    class Logger;
    class Server;
    class ServerSettings;

}

/**
 * @brief Класс ServerApplication предоставляет цикл обработки событий программы-сервера.
 *
 * Класс наследуется от QCoreApplication и дополняет его функционал в части обработки настроек,
 * вывода сообщений в лог-файл и управления объектом сервера.
 */
class ServerApplication : public QCoreApplication
{
    Q_OBJECT
    private:
        /**
         * @brief Регистратор сообщений, обеспечивающий их вывод в лог-файл
         */
        Core::Logger* logger_;

        /**
         * @brief Экземпляр сервера удаленного доступа
         */
        Core::Server* server_;

        /**
         * @brief Объект, обеспечивающий работу с настройками программы
         */
        Core::ServerSettings* settings_;

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param argc - количество параметров, передаваемых в программу из командной строки
         * @param argv - массив параметров, передаваемых в программу из командной строки
         *
         * Инициализирует экземпляр объекта, читает настройки программы, создает и запускает систему
         * вывода сообщений.
         *
         * Инициализирует систему вывода сообщений в соответствии с установленными настройками
         * локали в системе.
         *
         * После инициализации создает объект сервера и запускает его в режим ожидания
         *входящего соединения.
         */
        explicit ServerApplication(int & argc, char ** argv);

        /**
          * @brief Деструктор
          *
          * Останавливает сервер и уничтожает все созданные в конструкторе объекты.
          */
        ~ServerApplication();

        /**
         * @brief Возвращает указатель на регистратор сообщений
         * @return Указатель на регистратор сообщений
         */
        Core::Logger* logger() const
        {
            return logger_;
        }

        /**
         * @brief Возвращает указатель на экземпляр сервера удаленного доступа
         * @return Указатель на экземпляр сервера удаленного доступа
         */
        Core::Server* server() const
        {
            return server_;
        }

        /**
         * @brief Возвращает указатель на объект, обеспечивающий работу с настройками программы
         * @return Указатель на объект, обеспечивающий работу с настройками программы
         */
        Core::ServerSettings* settings() const
        {
            return settings_;
        }

        /**
         * @brief Устанавливает новый регистратор сообщений
         * @param logger - указатель на новый регистратор сообщений
         */
        void setLogger(Core::Logger *logger)
        {
            logger_ = logger;
        }

        /**
         * @brief Устанавливает новый сервер удаленного доступа
         * @param server - указатель на новый сервер удаленного доступа
         */
        void setServer(Core::Server *server)
        {
            server_ = server;
        }

        /**
         * @brief Заменяет объект, обеспечивающий работу с настройками программы
         * @param settings - указатель на новый объект, обеспечивающий работу с настройками программы
         */
        void setSettings(Core::ServerSettings *settings)
        {
            settings_ = settings;
        }
};

#endif // SERVER_APPLICATION_H
