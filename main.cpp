#include "simulation.h"
#include "trajectorywidget.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>

namespace {
bool readDoubleArgument(const QStringList &arguments, const QString &name, double *value)
{
    const QString prefix = name + "=";

    for (int i = 1; i < arguments.size(); ++i) {
        const QString argument = arguments[i];
        QString rawValue;

        if (argument == name && i + 1 < arguments.size()) {
            rawValue = arguments[i + 1];
        } else if (argument.startsWith(prefix)) {
            rawValue = argument.mid(prefix.size());
        } else {
            continue;
        }

        bool ok = false;
        const double parsedValue = rawValue.toDouble(&ok);
        if (!ok) {
            return false;
        }

        *value = parsedValue;
        return true;
    }

    return false;
}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const QStringList arguments = QCoreApplication::arguments();
    const bool noGui = arguments.contains("--no-gui");

    Simulation simulation;
    simulation.setupDefaultScenario();

    double angleDegrees = 45.0;
    double speed = 120.0;
    const bool angleFromArgs = readDoubleArgument(arguments, "--angulo", &angleDegrees);
    const bool speedFromArgs = readDoubleArgument(arguments, "--velocidad", &speed);

    if (arguments.contains("--manual") && !noGui && (!angleFromArgs || !speedFromArgs)) {
        bool ok = false;
        angleDegrees = QInputDialog::getDouble(nullptr,
                                               "Disparo manual",
                                               "Angulo en grados desde la horizontal:",
                                               angleDegrees,
                                               -89.0,
                                               89.0,
                                               1,
                                               &ok);
        if (!ok) {
            return 0;
        }

        speed = QInputDialog::getDouble(nullptr,
                                        "Disparo manual",
                                        "Rapidez inicial:",
                                        speed,
                                        1.0,
                                        500.0,
                                        1,
                                        &ok);
        if (!ok) {
            return 0;
        }
    }

    if (arguments.contains("--manual") || angleFromArgs || speedFromArgs) {
        simulation.setParticleLaunch(1, angleDegrees, speed);
    }

    simulation.run();

    const QString outputDir = QCoreApplication::applicationDirPath() + "/salida";
    const bool exportedText = simulation.exportTextFiles(outputDir);

    TrajectoryWidget window(simulation);
    const bool exportedImage = window.saveImage(outputDir + "/trayectorias.png");

    if (noGui) {
        return (exportedText && exportedImage) ? 0 : 1;
    }

    if (!exportedText || !exportedImage) {
        QMessageBox::warning(nullptr,
                             "Exportacion",
                             "La simulacion corrio, pero no se pudieron guardar todos los archivos de salida.");
    }

    window.show();
    return app.exec();
}
