#ifndef MULTIAXESCREWNUTDRIVE_H
#define MULTIAXESCREWNUTDRIVE_H

#include "device.h"

#include <QDebug>

class QTimer;
class PhytronMCC2;

const int MAX_AXES = 2;

/**
 * @brief The ScrewNutDrive class
 *
 * @todo Унифицировать работу с двигателем, чтобы можно было использовать контроллеры различных производителей!
 *
 * Класс ориентирован на работу с контроллером, поддерживающим несколько координат.
 * (PhytronMCC2)
 */
class MultiAxeScrewNutDrive : public Device
{
    Q_OBJECT
public:
    enum Axes
    {
        AXE_X = 0,
        AXE_Y = 1
    };

private:

    enum Direction
    {
        POSITIVE = 0,
        NEGATIVE = 1
    };


        //-------------------------------------
        // >> Аппаратно-зависимые переменные

        /**
         * @brief Ток двигателя, мА
         *
         * Значение по умолчанию 1200 мА
         */
        int driveCurrent_[MAX_AXES];

        /**
         * @brief Дробление шага двигателя
         *
         * Знчение по умолчанию 16
         */
        int driveStepRes_[MAX_AXES];

        /**
         * @brief Шагов на оборот двигателя
         *
         * Значение по умолчанию 200
         */
        int stepsForCircle_[MAX_AXES];

        int fastStepFreq_[MAX_AXES];                 //!< Частота шагов для быстрого движения по оси Z

        /**
         * @brief Максимально допустимое расстояние в положительном направлении, мм
         */
        double maxDistance_[MAX_AXES];

        int slowStepFreq_[MAX_AXES];                 //!< Частота шагов для медленного движения по оси Z

        /**
         * @brief Шаг пары винт/гайка, мм
         */
        double screwStep_[MAX_AXES];

        /**
         * @brief Шагов на 1 мм хода
         */
        int stepsForMm_[MAX_AXES];

        // << Аппаратно-зависимые переменные
        //-------------------------------------

        //-------------------------------
        // >> Исполнительные устройства

        PhytronMCC2 *phytronMCC2_;

        // << Исполнительные устройства
        //-------------------------------

        //-------------------------------
        // >> Состояния, специфичные для устройства

        /**
         * @brief Поиск нуля
         */
        QState *zeroSearching_;

        /**
         * @brief Привод остановлен
         */
        QState *stopped_;

        /**
         * @brief Привод в состоянии движения в целевую координату
         */
        QState *moving_;

        // << Состояния
        //-------------------------------

        /**
         * @brief Таймер, запускающий функцию контроля работы устройства
         */
        QTimer* timer_;

        /**
         * @brief Интервал таймера, контролирующего работу, мс
         *
         * Значение по умолчанию 500 мс
         */
        int timerInt_;

        /**
         * @brief Текущая позиция в мм (расчетная)
         *
         * Отсчет позиции ведется от нуля в положительном направлении
         */
        double pos_[MAX_AXES];

        /**
         * @brief Целевая позиция в мм
         *
         * Отсчет позиции ведется от нуля в положительном направлении
         */
        double nextPos_[MAX_AXES];

        /**
         * @brief Текущая позиция в мм (прочитанная с энкодера)
         *
         * Отсчет позиции ведется от нуля в положительном направлении
         *
         * @todo В настоящий момент считается по шагам двиагателя.
         * Энкодер не дал результата
         * Надо дать флаг использовать энкодер?
         */
        quint32 encPos_[MAX_AXES];

    public:
        explicit MultiAxeScrewNutDrive(PhytronMCC2 *&phytronMCC2, double screwStep, double maxDistance, QObject *parent = 0);

        bool isAtHome(bool &state, Axes axe);

        inline int timerInt() const;
        inline void setTimerInt(int timerInt);

        inline int driveCurrent(Axes axe = AXE_X) const;
        inline void setDriveCurrent(int driveCurrent, Axes axe = AXE_X);

        inline int driveStepRes(Axes axe = AXE_X) const;
        inline void setDriveStepRes(int driveStepRes, Axes axe = AXE_X);

        inline int stepsForCircle(Axes axe = AXE_X) const;
        inline void setStepsForCircle(int stepsForCircle, Axes axe = AXE_X);

        inline int fastStepFreq(Axes axe = AXE_X) const;
        inline void setFastStepFreq(int fastStepFreq, Axes axe = AXE_X);

        inline int slowStepFreq(Axes axe = AXE_X) const;
        inline void setSlowStepFreq(int slowStepFreq, Axes axe = AXE_X);

        inline double screwStep(Axes axe = AXE_X) const;
        inline void setScrewStep(double screwStep, Axes axe = AXE_X);

        inline int stepsForMm(Axes axe = AXE_X) const;

        inline double maxDistance(Axes axe = AXE_X) const;
        inline void setMaxDistance(double maxDistance, Axes axe = AXE_X);

        inline qint32 stepsForDistance(double distance, Axes axe = AXE_X) const;

    public slots:

        /**
         * @brief Переместить привод в положение нулевого выключателя
         */
        void goHome();

        /**
         * @brief Переместить привод на заданное расстояние в положительном направлении
         * @param distance - расстояние перемещения в мм
         */
        void moveNegative(double distance, MultiAxeScrewNutDrive::Axes axe);

        /**
         * @brief Переместить привод на заданное расстояние в отрицательном направлении
         * @param distance - расстояние перемещения в мм
         */
        void movePositive(double distance, MultiAxeScrewNutDrive::Axes axe);

        /**
         * @brief moveTo
         * @param x
         * @param y
         */
        void moveTo(double x, double y);

        void moveTo(double coord, MultiAxeScrewNutDrive::Axes axe);

        /**
         * @brief Остановить движение
         */
        void stop();

private slots:

    void stoppedEntered_();

    /**
     * @brief Проверить завершение выполнения команды движения
     *
     * Вызывается по таймеру, запускаемому после передачи контроллеру команды движения.
     * В зависимости от результата проверки переключает состояния.
     */
    void checkMoving_();

    virtual void failedEntered_();

    void initFinished_();

    /**
     * @brief Вызывается при входе состояние инициализации
     *
     * Запускает процесс инициализации, устанавливая параметры двигателя
     * и давая команду, чтобы гарантированно покинуть точку нуля.
     */
    void initStarted_();

    void movingStarted_();

    /**
     * @brief Вызывается при входе в состояние поиска нуля
     *
     * Выдает команду на поворот руки против часов на малый угол
     * и связывает сигнал контроллера о достижении целевой координаты
     * со слотом обработки.
     */
    void zeroSearchStarted_();

signals:
    void movingDone();
};


int MultiAxeScrewNutDrive::timerInt() const
{
    return timerInt_;
}

void MultiAxeScrewNutDrive::setTimerInt(int timerInt)
{
    timerInt_ = timerInt;
}

int MultiAxeScrewNutDrive::driveCurrent(Axes axe) const
{
    return driveCurrent_[axe];
}

void MultiAxeScrewNutDrive::setDriveCurrent(int driveCurrent, Axes axe)
{
    driveCurrent_[axe] = driveCurrent;
}

int MultiAxeScrewNutDrive::driveStepRes(Axes axe) const
{
    return driveStepRes_[axe];
}

void MultiAxeScrewNutDrive::setDriveStepRes(int driveStepRes, Axes axe)
{
    driveStepRes_[axe] = driveStepRes;

    if (screwStep_[axe] != 0.0)
        stepsForMm_[axe] = double(stepsForCircle_[axe]) * double(driveStepRes_[axe]) / screwStep_[axe];
    else
        stepsForMm_[axe]  = 0;
}

int MultiAxeScrewNutDrive::stepsForCircle(Axes axe) const
{
    return stepsForCircle_[axe];
}

void MultiAxeScrewNutDrive::setStepsForCircle(int stepsForCircle, Axes axe)
{
    stepsForCircle_[axe] = stepsForCircle;

    if (screwStep_[axe] != 0.0)
        stepsForMm_[axe] = double(stepsForCircle_[axe]) * double(driveStepRes_[axe]) / screwStep_[axe];
    else
        stepsForMm_[axe]  = 0;
}

int MultiAxeScrewNutDrive::fastStepFreq(Axes axe) const
{
    return fastStepFreq_[axe];
}

void MultiAxeScrewNutDrive::setFastStepFreq(int fastStepFreq, Axes axe)
{
    fastStepFreq_[axe] = fastStepFreq;
}

int MultiAxeScrewNutDrive::slowStepFreq(Axes axe) const
{
    return slowStepFreq_[axe];
}

void MultiAxeScrewNutDrive::setSlowStepFreq(int slowStepFreq, Axes axe)
{
    slowStepFreq_[axe] = slowStepFreq;
}

double MultiAxeScrewNutDrive::screwStep(Axes axe) const
{
    return screwStep_[axe];
}

//-----------------------------------------------------------------------------

int
MultiAxeScrewNutDrive::stepsForMm(Axes axe) const
{
    return stepsForMm_[axe];
}

double MultiAxeScrewNutDrive::maxDistance(Axes axe) const
{
    return maxDistance_[axe];
}

void MultiAxeScrewNutDrive::setMaxDistance(double maxDistance, Axes axe)
{
    maxDistance_[axe] = maxDistance;
}

qint32 MultiAxeScrewNutDrive::stepsForDistance(double distance, Axes axe) const
{
    return double(stepsForMm_[axe]) * distance;
}


#endif // MULTIAXESCREWNUTDRIVE_H
