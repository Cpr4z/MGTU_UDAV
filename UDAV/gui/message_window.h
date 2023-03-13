#ifndef MESSAGE_WINDOW_H
#define MESSAGE_WINDOW_H

#include <QWidget>

class QPlainTextEdit;
class QTableWidget;
class QPushButton;
class QTimer;

namespace udav_gui
{

/**
 * @brief Класс окна для вывода пользователю сообщений, поступающих от сервера.
 *
 * Окно создается клиентом автоматически при поступлении команды на открытие окна.
 *
 * Вывод сообщений в окно производится по его идентификатору.
 *
 * До момента завершения вывода сообщений закрытие окна пользователем блокируется.
 * После вывода сообщений сервер или дает команду на закрытие окна, или разрешает
 * пользователю закрыть его вручную.
 */
class MessageWindow : public QWidget
{
    Q_OBJECT
    private:

        /**
         * @brief Флаг для определения режима закрытия окна: автоматически или пользователем
         *
         * Значение по умолчанию - true
         */
        bool autoClose_;

        /**
         * @brief Флаг для определения возможности закрытия окна
         *
         * Значение по умолчанию - false. Устанавливается в true после выполнения closeWindow().
         */
        bool canClose_;

        /**
         * @brief Кнопка отключения фильтрации сообщений
         */
        QPushButton* allButton_;

        /**
         * @brief Кнопка закрытия окна
         */
        QPushButton* closeButton_;

        /**
         * @brief Таймер для контроля времени задержки закрытия окна
         */
        QTimer* closeTimer_;

        /**
         * @brief Кнопка включения фильтрации критических сообщений
         */
        QPushButton* errorButton_;

        /**
         * @brief Область для вывода сообщений
         */
        QTableWidget* messageBrowser_;

        /**
         * @brief Счетчик выведенных сообщений
         */
        int rowCount_;

    private slots:

        /**
         * @brief Закрыть окно или сделать доступной кнопку закрытия в зависимости от значения флага autoClose_
         *
         */
        void closeWindow_();

        /**
         * @brief Показать в области вывода сообщений все сообщения, поступившие от сервера
         * @param state
         */
        void showAll_(bool state);

        /**
         * @brief Показать в области вывода сообщений только сообщения о возникших ошибках
         * @param state
         */
        void showErrors_(bool state);

    protected:

        virtual void closeEvent(QCloseEvent *event);

        virtual void resizeEvent(QResizeEvent *event);

    public:

        /**
         * @brief Конструктор по умолчанию
         * @param parent - окно-владелец
         */
        explicit MessageWindow(QWidget *parent = 0);

    public slots:

        /**
         * @brief Обработать команду на закрытие окна
         * @param delay - время задержки закрытияв секундах
         * @param autoClose - закрыть автоматически или сделать доступной пользователю кнопку закрытия
         *
         * Окно будет закрыто с задержкой delay.
         *
         * Если autoClose равно false, то окно закрыто не будет, а пользователю
         * станед доступна кнопка закрытия.
         */
        void closeWindow(quint16 delay, bool autoClose_);

        /**
         * @brief Вывести сообщение в область вывода сообщений
         * @param type - тип сообщения (@sa Core::MessageType)
         * @param message - стока сообщения
         */
        void showMessage(quint16 type, const QString& message);

    signals :

        /**
         * @brief Cообщить о закрытии окна
         * @param widget
         *
         * Сигнал может быть обработан для удаления объекта окна его владельцем.
         */
        void windowClosed(QWidget* widget);

};

}

#endif // MESSAGE_WINDOW_H
