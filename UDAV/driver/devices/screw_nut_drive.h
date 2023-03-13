#ifndef SCREWNUTDRIVE_H
#define SCREWNUTDRIVE_H

#include "device.h"


class QTimer;
class OnitexOSM;

//=============================================================================


/**
 * @brief Привод винт-гайка с шаговым двигателем
 *
 * @todo Унифицировать работу с двигателем, чтобы можно было использовать контроллеры различных производителей!
 *
 */
class ScrewNutDrive : public Device
{
    Q_OBJECT
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
        int driveCurrent_;

        /**
         * @brief Дробление шага двигателя
         *
         * Знчение по умолчанию 16
         */
        int driveStepRes_;

        /**
         * @brief Шагов на оборот двигателя
         *
         * Значение по умолчанию 200
         */
        int stepsForCircle_;

        int fastStepFreq_;                 //!< Частота шагов для быстрого движения по оси Z

        /**
         * @brief Максимально допустимое расстояние в положительном направлении, мм
         */
        double maxDistance_;

        int slowStepFreq_;                 //!< Частота шагов для медленного движения по оси Z

        /**
         * @brief Шаг пары винт/гайка, мм
         */
        double screwStep_;

        /**
         * @brief Шагов на 1 мм хода
         */
        int stepsForMm_ ;

        // << Аппаратно-зависимые переменные
        //-------------------------------------

        //-------------------------------
        // >> Исполнительные устройства

        OnitexOSM *onitexOSM_;

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
         * Отсчет позиции ведется от нуля
         */
        double pos_;

        /**
         * @brief Целевая позиция в мм
         *
         * Отсчет позиции ведется от нуля
         */
        double nextPos_;

        /**
         * @brief Текущая позиция в мм (прочитанная с энкодера)
         *
         * Отсчет позиции ведется от нуля в положительном направлении
         *
         * @todo В настоящий момент не используется. Надо дать флаг использовать энкодер?
         */
        qint32 encPos_;


    public:
        explicit ScrewNutDrive(OnitexOSM *&onitexOSM, double screwStep, double maxDistance, QObject *parent = 0);

        bool isAtHome(bool &state);

        inline int timerInt() const;
        inline void setTimerInt(int timerInt);

        inline int driveCurrent() const;
        inline void setDriveCurrent(int driveCurrent);

        inline int driveStepRes() const;
        inline void setDriveStepRes(int driveStepRes);

        inline int stepsForCircle() const;
        inline void setStepsForCircle(int stepsForCircle);

        inline int fastStepFreq() const;
        inline void setFastStepFreq(int fastStepFreq);

        inline int slowStepFreq() const;
        inline void setSlowStepFreq(int slowStepFreq);

        inline double screwStep() const;
        inline void setScrewStep(double screwStep);

        inline int stepsForMm() const;

        inline double maxDistance() const;
        inline void setMaxDistance(double maxDistance);

        inline quint32 stepsForDistance(double distance) const;

    public slots:

        /**
         * @brief Переместить привод в положение нулевого выключателя
         */
        void goHome();

        /**
         * @brief Переместить привод на заданное расстояние в положительном направлении
         * @param distance - расстояние перемещения в мм
         */
        void moveNegative(double distance);

        /**
         * @brief Переместить привод на заданное расстояние в отрицательном направлении
         * @param distance - расстояние перемещения в мм
         */
        void movePositive(double distance);

        /**
         * @brief Переместить привод в заданную координату
         * @param coord  - целевая координата в мм
         */
        void moveTo(double coord);

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


int ScrewNutDrive::timerInt() const
{
    return timerInt_;
}

void ScrewNutDrive::setTimerInt(int timerInt)
{
    timerInt_ = timerInt;
}

int ScrewNutDrive::driveCurrent() const
{
    return driveCurrent_;
}

void ScrewNutDrive::setDriveCurrent(int driveCurrent)
{
    driveCurrent_ = driveCurrent;
}

int ScrewNutDrive::driveStepRes() const
{
    return driveStepRes_;
}

void ScrewNutDrive::setDriveStepRes(int driveStepRes)
{
    driveStepRes_ = driveStepRes;

    if (screwStep_ != 0.0)
        stepsForMm_ = double(stepsForCircle_) * double(driveStepRes_) / screwStep_;
    else
        stepsForMm_  = 0;
}

int ScrewNutDrive::stepsForCircle() const
{
    return stepsForCircle_;
}

void ScrewNutDrive::setStepsForCircle(int stepsForCircle)
{
    stepsForCircle_ = stepsForCircle;

    if (screwStep_ != 0.0)
        stepsForMm_ = double(stepsForCircle_) * double(driveStepRes_) / screwStep_;
    else
        stepsForMm_  = 0;
}

int ScrewNutDrive::fastStepFreq() const
{
    return fastStepFreq_;
}

void ScrewNutDrive::setFastStepFreq(int fastStepFreq)
{
    fastStepFreq_ = fastStepFreq;
}

int ScrewNutDrive::slowStepFreq() const
{
    return slowStepFreq_;
}

void ScrewNutDrive::setSlowStepFreq(int slowStepFreq)
{
    slowStepFreq_ = slowStepFreq;
}

double ScrewNutDrive::screwStep() const
{
    return screwStep_;
}

void ScrewNutDrive::setScrewStep(double screwStep)
{
    screwStep_ = screwStep;

    if (screwStep_ != 0.0)
        stepsForMm_ = double(stepsForCircle_) * double(driveStepRes_) / screwStep_;
    else
        stepsForMm_  = 0;
}

int ScrewNutDrive::stepsForMm() const
{
    return stepsForMm_;
}

double ScrewNutDrive::maxDistance() const
{
    return maxDistance_;
}

void ScrewNutDrive::setMaxDistance(double maxDistance)
{
    maxDistance_ = maxDistance;
}

quint32 ScrewNutDrive::stepsForDistance(double distance) const
{
    return double(stepsForMm_) * distance;
}


#endif // SCREWNUTDRIVE_H
