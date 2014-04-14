#include "zonotope.h"
using namespace std;

Zonotope::Zonotope(QString x, QString y, QString z){
    this->x = x;
    this->y = y;
    this->z = z;
    dynamic = false;
    transform(x,y,z);
}

Zonotope::Zonotope(QString x, QString y, QString z, bool Dynamic){
    this->x = x;
    this->y = y;
    this->z = z;
    dynamic = Dynamic;
    if (dynamic){
        transform_dynamic(x,y,z);
    }
    else
        Zonotope(x, y, z);
}

void Zonotope::transform_dynamic(QString x, QString y, QString z){
    transform(x,y,z);
    double*** tcoeff = new double**[3];
    tcoeff[0] = trans_dynamic(x);
    tcoeff[1] = trans_dynamic(y);
    tcoeff[2] = trans_dynamic(z);

    int ncount[53];
    numberofdynamicvariable = 0;
    for (int i = 0; i <= 52; i++)
        ncount[i] = 0;
    for (int i = 0; i < 52; i++){
        for (int j = 0; j < MAX_EXPONENT; j++)
            if (tcoeff[0][i][j] != 0 ||
                    tcoeff[1][i][j] != 0 ||
                    tcoeff[2][i][j] != 0)
            {
                ncount[i] = 1;
                numberofdynamicvariable ++;
                break;
            }
    }

    if (numberofdynamicvariable == 0)
        return;
    coeff_dynamic = new double**[3];
    for (int i = 0; i < 3; i++){
        coeff_dynamic[i] = new double* [numberofdynamicvariable+1];
        for (int j = 0; j <= numberofdynamicvariable; j++){
            coeff_dynamic[i][j] = new double[MAX_EXPONENT];
            for (int k = 0; k < MAX_EXPONENT; k++)
                coeff_dynamic[i][j][k] = 0;
        }
    }

    for (int i = 0; i < 3; i++){
        int index = 0;
        for (int j = 0; j < 52; j++){
            if (ncount[j] == 1){
                for (int k = 0; k < MAX_EXPONENT; k++)
                    coeff_dynamic[i][index][k] = tcoeff[i][j][k];
                index++;
            }
        }
        for (int k = 0; k < MAX_EXPONENT; k++)
            coeff_dynamic[i][index][k] = tcoeff[i][52][k];
    }

    numberoftotalvariable = numberofvariable + numberofdynamicvariable;
    if (numberoftotalvariable > 52){
        numberofdynamicvariable = 0;
        return;
    }

    double** coeff_to_update = new double* [3];
    for (int i = 0; i < 3; i++){
        coeff_to_update[i] = new double [numberoftotalvariable+1];
        for (int j = 0; j <= numberoftotalvariable; j++)
            coeff_to_update[i][j] = 0;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < numberofvariable; j++)
            coeff_to_update[i][j] = coeff[i][j];

    coeff = coeff_to_update;
}

void Zonotope::coeff_update(double time){
    for (int i = 0; i < 3; i++)
        for (int j = numberofvariable; j <= numberoftotalvariable; j++){
            double t = 1;
            double sum = 0;
            for (int k = 0; k < MAX_EXPONENT; k++){
                sum += coeff_dynamic[i][j-numberofvariable][k] * t;
                t *= time;
            }
            coeff[i][j] = sum;
        }
}

double** Zonotope::trans_dynamic(QString x){
    int len = x.length();
    double **a = new double* [53];
    for (int i = 0; i < 53; i++){
        a[i] = new double[MAX_EXPONENT];
        for (int j = 0; j < MAX_EXPONENT; j++)
            a[i][j] = 0;
    }

    for (int i = 0; i < len; i++){
        while (i < len && x[i].toAscii() != '(')
            i++;
        i++;
        if (i >= len)
            break;

        int status = 0; // 0 for start of coeff, 1 for start of exponent;
        int currentflag = 1;
        int currentexponent = 0;
        bool dot = false;
        double dotbit = 10;
        double currentcoeff = 0;
        double coefftemp[MAX_EXPONENT];
        for (int k = 0; k < MAX_EXPONENT; k++)
            coefftemp[k] = 0;

        for (; i < len; i++){
            QChar ctemp = x[i];
            if (ctemp.toAscii() == ')'){
                if (currentcoeff == 0)
                    currentcoeff = 1;
                if (status == 2)
                    coefftemp[currentexponent] += currentcoeff * currentflag;
                else if (status == 1)
                    coefftemp[1] += currentcoeff * currentflag;
                else if (status == 0)
                    coefftemp[0] += currentcoeff * currentflag;
                break;
            }
            if (ctemp.toAscii() == '.'){
                dot = true;
                continue;
            }

            if (!ctemp.isLetterOrNumber() && ctemp.toAscii() != '^'){
                if (currentcoeff == 0)
                    currentcoeff = 1;
                if (status == 2)
                    coefftemp[currentexponent] += currentcoeff * currentflag;
                else if (status == 1)
                    coefftemp[1] += currentcoeff * currentflag;
                else if (status == 0)
                    coefftemp[0] += currentcoeff * currentflag;
                status = 0;
                currentcoeff = 0;
                currentexponent = 0;
                currentflag = 1;
                dot = false;
                dotbit = 10;
                if (ctemp.toAscii() == '-')
                    currentflag = -1;
                continue;
            }

            if (ctemp.toAscii() == 't'){
                status = 1;
                continue;
            }
            if (status == 1 && ctemp.toAscii() == '^'){
                ctemp = x[++i];
                currentexponent = ctemp.toAscii() - '0';
                status = 2;
                if (currentexponent < 0 || currentexponent >= MAX_EXPONENT){
                    currentexponent = 0;

                }
                continue;
            }
            if (ctemp.isDigit()){
                if (!dot)
                    currentcoeff = currentcoeff * 10 + ctemp.toAscii() - '0';
                else{
                    currentcoeff += (ctemp.toAscii() - '0')/dotbit;
                    dotbit *= 10;
                }
            }
        }

        if (x[i].toAscii() == ')'){
            i++;
            int index = -1;
            if (i == len){
                index = 52;
            }
            else{
                if (x[i].isLower())
                    index = x[i].toAscii() - 'a';
                else if (x[i].isUpper())
                    index = x[i].toAscii() - 'A';
            }
            if (index < 0 || index > 52)
                break;
            for (int k = 0; k < MAX_EXPONENT; k++)
                a[index][k] = coefftemp[k];
        }
    }

    return a;
}

int Zonotope::getnumberofpoints(){
    // Return number of points, 2^(numberofvariable)
    if (!dynamic)
        return 1<<numberofvariable;
    else
        return 1<<numberoftotalvariable;
}

int Zonotope::getnumberofvariable(){
    // Return the number of variables
    if (!dynamic)
        return numberofvariable;
    else
        return numberoftotalvariable;
}


double **Zonotope::getcoeff(double time){
    if (!dynamic || time > tmax)
        return coeff;
    else{
        coeff_update(time);
        return coeff;
    }
}


double** Zonotope::getcoeff(){
    // Return the coefficients
    return coeff;
}

vector<Face> Zonotope::getAllFace(){
    QuickHull3D *hull = new QuickHull3D(getpoints(), getnumberofpoints());
    vector<Point3D *> vertices = hull->getVertices();
    vector<vector<int> > faceIndices = hull->getFaces();

    vector<Face> vf;
    for (int i = 0; i < faceIndices.size(); i++){
        Face f = Face();
        for (int j = 0; j < faceIndices[i].size(); j++){
            Point3D *temp = vertices[faceIndices[i][j]];
            f.addpoint(Point(temp->x, temp->y, temp->z));
        }
        vf.push_back(f);
    }

    return vf;
}

void Zonotope::transform(QString x, QString y, QString z){
    // Transform three equations

    // Transform each equation
    double** tcoeff = new double*[3];
    tcoeff[0] = trans(x);
    tcoeff[1] = trans(y);
    tcoeff[2] = trans(z);

    // Count the number of variables
    int a[52];
    for (int i = 0; i < 52; i++)
        a[i] = 0;
    for (int i = 0; i < 52; i++)
        for (int j = 0; j < 3; j++)
            if (tcoeff[j][i] != 0)
                a[i] = 1;
    numberofvariable = 0;
    for (int i = 0; i < 52; i++)
        if (a[i] != 0)
            numberofvariable++;

    // Copy the corresponding coefficients, the last one is the constant
    coeff = new double*[3];
    for (int i = 0; i < 3; i++)
        coeff[i] = new double[numberofvariable+1];
    for (int i = 0; i < 3; i++){
        int index = 0;
        for (int k = 0; k < 52; k++){
            if (a[k] != 0)
                coeff[i][index++] = tcoeff[i][k];
        }
        coeff[i][index] = tcoeff[i][52];
    }
}

double* Zonotope::trans(QString x){
    // Transform each equation
    // Legal variables are (a-z, A-Z)
    // The maximum number of variables is 52
    // Plus one constant

    int len = x.length();
    double *a = new double[53];
    for (int i = 0; i < 53; i++)
        a[i] = 0;
    double currentcoeff = 0;
    int currentflag = 1;
    bool dot = false;
    double dotbit = 10;
    int status = 0; // 0:Previous caracter is an variable, 1:Previous caracter is an number.
    for (int i = 0; i < len; i++){
        QChar temp = x[i];
        if (temp.toAscii() == '('){
            while (x[i].toAscii() != ')')
                i++;
            i += 1;
            continue;
        }
        if (temp.toAscii() == '.'){
            dot = true;
            continue;
        }
        if (temp.isLetter()){
            if (temp.isLower())
                a[temp.toAscii()-'a'] += currentcoeff==0 ? currentflag:currentflag*currentcoeff;
            else if (temp.isUpper())
                a[temp.toAscii() -'A'+26] += currentcoeff==0 ? currentflag:currentflag*currentcoeff;
            status = 0;
            dot = false;
            dotbit = 10;
            currentcoeff = 0;
            currentflag = 1;
            continue;
        }
        if (!temp.isLetterOrNumber()){
            // If current caracter is not variable or number
            // And the previous caracter is not variable, so the coefficient is constant
            if (status == 1)
                a[52] += currentflag*currentcoeff;
            currentcoeff = 0;
            dot = false;
            dotbit = 10;
            if (temp.toAscii() == '-')
                currentflag= -1;
            continue;
        }
        if (temp.isDigit()){
            status = 1;
            if (!dot)
                currentcoeff = currentcoeff * 10 + temp.toAscii() - '0';
            else{
                currentcoeff += (temp.toAscii() - '0')/dotbit;
                dotbit *= 10;
            }
            if (i == len-1)
                a[52] += currentflag*currentcoeff;
        }
    }
    return a;
}

double** Zonotope::getpoints(){
    // Return all the points
    // 2^n in total

    int npoint = getnumberofpoints();
    int nvariable = getnumberofvariable();
    double ** points = new double*[npoint];
    for (int i = 0; i < npoint; i++)
        points[i] = new double[3];
    double* tcoeff = new double[nvariable];
    for(int i = 0; i < npoint; i++){
        int temp = i;
        for (int j = 0; j < nvariable; j++){
            tcoeff[j] = temp & 1;
            temp = temp>>1;
        }
        for (int k = 0; k < 3; k++){
            points[i][k] = multi(coeff[k], tcoeff, nvariable)
                    + coeff[k][nvariable];
        }
    }
    return points;
}

double Zonotope::multi(double* a, double* b, int d){
    double sum = 0;
    for (int i = 0; i < d; i++)
        sum += a[i]*b[i];
    return sum;
}

vector<Face> generateface(double** points, int n){
    // Generate faces from points, Version 1 (Brutal Force)
    // Runtime complexity is O(n^4)
    // Further work: reduce the runtime complexity
    // Three steps:
    //      1:Find how many faces these points can form
    //      2:Delete the duplicated faces
    //      3:Find whether all the points are at the same side of the face,
    //            if not, delete this face.

    // Step 1
    vector<Face> vf;
    for (int i = 0; i < n-2; i++){
        for (int j = i+1; j < n-1; j++){
            for (int k = j+1; k < n; k++){
                Face ftemp;
                ftemp.addpoint(Point(points[i]));
                ftemp.addpoint(Point(points[j]));
                ftemp.addpoint(Point(points[k]));
                for (int m = k+1; m < n; m++){
                    if ((!ftemp.isface()) ||(ftemp.relativeposition(Point(points[m])) == 0))
                        ftemp.addpoint(Point(points[m]));
                }
                if (ftemp.isface()){
                    vf.push_back(ftemp);
                }
            }
        }
    }

    // Step 2
    int size = vf.size();
    vector<Face> vf2;
    for (int i = 0; i < size; i++){
        if (vf2.empty()){
            vf2.push_back(vf[i]);
            continue;
        }
        Face ftemp = vf[i];
        int vf2size = vf2.size();
        int j;
        for (j = 0; j < vf2size; j++){
            if (ftemp.isequal(vf2[j]))
                break;
        }
        if (j == vf2size)
            vf2.push_back(ftemp);
    }

    // Step 3
    vector<Face> vf3;
    size = vf2.size();
    for (int i = 0; i < size; i++){
        int rtposition = 0;
        Face ftemp = vf2[i];
        int j;
        for (j = 0; j < n; j++){
            int currentposition = ftemp.relativeposition(Point(points[j]));
            if (rtposition == 0){
                rtposition = currentposition;
                continue;
            }
            if (currentposition*rtposition < 0)
                break;
        }
        if (j == n)
            vf3.push_back(ftemp);
    }

    return vf3;
}

vector<Face> generateface2(double** points, int n){
    // Generate faces from points, Version 2 (Brutal Force)
    // Runtime complexity is O(n^4)
    // Further work: reduce the runtime complexity
    // Two steps:
    //      1:Find how many faces these points can form,
    //            ignore the duplicated faces.
    //      2:Find whether all the points are at the same side of the face,
    //            if not, delete this face.

    // Step 1
    vector<Face> vf;
    int count = 0;
    for (int i = 0; i < n-2; i++){
        for (int j = i+1; j < n-1; j++){
            bool ischecked = false;
            bool exist = false;
            for (int k = j+1; k < n; k++){
                Face ftemp;
                ischecked = false;
                exist = false;
                ftemp.clear();
                ftemp.addpoint(Point(points[i]));
                ftemp.addpoint(Point(points[j]));
                ftemp.addpoint(Point(points[k]));
                for (int m = k+1; m < n; m++){
                    if ((!ftemp.isface()) ||(ftemp.relativeposition(Point(points[m])) == 0))
                        ftemp.addpoint(Point(points[m]));
                    if (!exist && !ischecked && ftemp.isface()){
                        int currentsize = vf.size();
                        ischecked = true;
                        for (int cp = 0; cp < currentsize; cp++){
                            if (ftemp.isequal(vf[cp])){
                                exist = true;
                                break;
                            }
                        }
                    }
                    if (exist)
                        break;
                }
                if (!ftemp.isface())
                    break;
                if (!ischecked){
                    int currentsize = vf.size();
                    ischecked = true;
                    for (int cp = 0; cp < currentsize; cp++){
                        if (ftemp.isequal(vf[cp])){
                            exist = true;
                            break;
                        }
                    }
                }
                if (!exist && ftemp.isface()){
                    count++;
                    vf.push_back(ftemp);
                }
            }
        }
    }

    // Step 2
    int size = vf.size();
    vector<Face> vfin;
    for (int i = 0; i < size; i++){
        int rtposition = 0;
        Face ftemp = vf[i];
        int j;
        for (j = 0; j < n; j++){
            int currentposition = ftemp.relativeposition(Point(points[j]));
            if (rtposition == 0){
                rtposition = currentposition;
                continue;
            }
            if (currentposition*rtposition < 0)
                break;
        }
        if (j == n)
            vfin.push_back(ftemp);
    }

    return vfin;
}

void printface(vector<Face> vf){
    // Print the face vector

    int size = vf.size();
    for (int i = 0; i < size; i++){
        Face ftemp = vf[i];
        qDebug("The %d face:", i);
        double* para = ftemp.getpara();
        qDebug("Para: %.2f, %.2f, %.2f, %.2f", para[0],para[1],para[2],para[3]);
        int fsize = ftemp.getsize();
        vector<Point> vpo = ftemp.getvp();
        for (int i = 0; i < fsize; i++){
            double* pointtemp = vpo[i].getcoor();
            qDebug("%.2f, %.2f, %.2f", pointtemp[0], pointtemp[1], pointtemp[2]);
        }
    }
}

double** normalize(double** points, int n){
    double ax = 0, ay = 0, az = 0;
    for (int i = 0; i < n; i++){
        ax += points[i][0];
        ay += points[i][1];
        az += points[i][2];
    }
    ax /= n; ay /= n; az /= n;
    for (int i = 0; i < n; i++){
        points[i][0] -= ax;
        points[i][1] -= ay;
        points[i][2] -= az;
    }
    return points;
}
