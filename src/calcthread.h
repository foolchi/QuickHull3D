#ifndef CALCTHREAD_H
#define CALCTHREAD_H
/**
 * @file calcthread.h
 * @brief QThread for sleep and calculation
 *
 * Sleep thread for waiting for a minimal time step
 * Calculation thread for calculation without affect the GUI
 *
 */

#include <face.h>
#include <zonotope.h>

/**
 * @brief The SleepThread class
 *
 * Simple sleep thread for waiting
 */
class SleepThread : public QThread{
    Q_OBJECT
public slots:

    /**
     * @brief Sleep for given millisecond
     * @param millisecond
     */
    static void msleep(unsigned long msecs){
        //qDebug("Sleeping");
        QThread::msleep(msecs);
    }


};


/**
 * @brief The CalcThread class
 *
 * Thread for calculation the faces
 */
class CalcThread: public QThread{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param Zonotope for calculation
     */
    CalcThread(Zonotope zp){
        updated = true;
        this->zp = zp;
        tmax = zp.getmaxtime();
        tstep = zp.gettimestep();
    }

    /**
     * @brief Stop the thread
     */
    void stop();

    /**
     * @brief Pause the thread
     */
    void pause();

public slots:

    /**
     * @brief Check if the UI has been updated
     *
     * Receive the signal from UI when the UI has been updated
     */
    void faceupdated(){
        updated = true;
    }

signals:

    /**
     * @brief Send a signal if the calculation is finished
     * @param Pointer to vector<Face>
     *
     * Send a signal to UI if the calculation is finished
     * Using a pointer of int because vector<Face> is a class
     */
    void faceChanged(int* vf);

protected:

    /**
     * @brief Run the thread
     *
     * Calculate and update the face information for a given period
     */
    void run(){
        SleepThread spthread;
        for (double t = tstep; t <= tmax; t += tstep){
            if (updated){
                zp.coeff_update(t);
                vf = zp.getAllFace();
                updated = false;
                emit faceChanged((int *) (&vf));
            }
            spthread.msleep((unsigned(tstep*1000)));
        }
    }


private:
    bool updated; /*!< Check if the UI has been updated */
    double tstep, tmax; /*!< Time step and max time */
    Zonotope zp; /*< Zonotope for calculation */
    vector<Face> vf; /*< Faces to be painted */
};

#endif // CALCTHREAD_H
