
// This code is released and free to use for personal purposes. 
// This is one set of code in a larger development platform.
//
// Please send all comments, suggestions, improvements, bug reports, etc. to: robnelsonxx2@gmail.com
//
// Some examples created using this code and its GUI-based development platform, DavCNN: 
//
//      https://www.youtube.com/watch?v=cUe2vyS2gJg     (SoftMax Example, Surrounding Data)
//      https://www.youtube.com/watch?v=V745YDkmoew     (Neural Network Learning a Trigonometric Function)
//
// Gfycat versions:
//
//      https://gfycat.com/scarywelltodoflickertailsquirrel     (SoftMax Example, Surrounding Data)
//      https://gfycat.com/bothelaboratearachnid                (Neural Network Learning a Trigonometric Function)
//
// This neural network code has been very useful for development, and hopefully I'll get some feedback so I can make it more useful and faster as
// time goes by.
//
// I'd also like to release the convolutional neural network code I'm working on, as well as the core graphics and development engines used for 
// creating the samples I release so far. 
//
// DavCNN.CPP (not released yet)
//
// The core development and graphics engine, DavCNN would allow for graphic output as well as GUI-based development and diagnostic tools such as
// a realtime representation of the neural network, allowing for working witn the neural network as it is training.  Such things as turning off/freezing
// current weights and nodes, as well as adding and removing nodes, shifting and modifying the training set, etc. have been very useful.

// Overview of CDevNN.CPP
//
// CDevNN.CPP is a small, fast NN engine that performs forward and backward propagation on multi-layer neural networks.  It is a personal engine designed to work with
// general and convolutional neural networks, with development control and analysis during training. 
//
// note: there may be some test-code left in this file.  Also excuse any formatting errors caused by accidental tab spacing.
//
// Overview of the code
// --------------------
// 
// There are four primary parts to CDevNN.CPP:
//
//      Train()             -- Train the neural network, puting together all of the basic functionality
//      Forward()           -- Forward Propagation
//      Backward()          -- Backward Proppagation
//      AdjustLearnRate()   -- Adjust the learning rate dynamically
//
// The code is written to be self-documenting and is an engine that is used internally, called by other processes.   As such, there are certain
// elements that would change as a standalone version.  A few examples:
//
//          1. Elements such as the error-text would be converted to const declarations are left in place to keep the self-documentation
//             quality. As a standalone version, the error return would also be an error code rather than an error message.  But it is more
//             useful for development in its current form.
//          2. Also, the declared type <double> would either used as a typedef or template, to allow other compilers and native types
//             (i.e. float, double, long double, etc.)
//          3. For speed, some code is not error-checked or validated, and assumes that you know what you're doing if you're in the
//             depths of the code, where otherwise it might error-out
//          4. The exception handling is an internal structure that is very useful for developing, self-documenation, avoiding early-exits and/or layers of
//             conditional code.  It also allows profiling and debugging in an non-intrusive manner while allowing it to be compiled-out in a release
//             version.  This wouldn't change in a release version, but would be more standardized.
//          5. More functions specific to intel-based and MSVC platforms would be abstracted, namely the memory allocation and deallocation calls.
//
// Speeding up CDevNN.CPP +x10x with AVX and Multi-processing.
// --------------------------------------------------------
//
// Another release for CDevNN.CPP would be the AVX/multi-processor version
//
// CDevNN.CPP a full C++ version of the CDevNN code, without AVX instrinsics or multi-processing support.
// See notes on the Forward() and Backward() functions.  As well as being faster, AVX code allows for a much smaller amount of code for most operations, allowing
// better L1 caching and faster code in additon multiple value operations.
//
// With multi-processing, its a simple task to assign more processors (important mostly for the backward() function) when appropriate (for example, after the first
// or second layer in the Backward() function), as well as, perhaps, duplicating certain memory arrays to give unique memory to each process.
// 
// See notes on the commented-out version of the Forward() function for more information


#include "CDevString.h"
#include <math.h>
#include <io.h>
#include <float.h>

#pragma once

#pragma warning(disable:4351)       // Turn off {} initializer warning for MSVC (some sort of legacy issue)

// Exception Handling
// 
// Local exception handling defines.  As mentioned in the introduction, these return messages for clarity in the code, 
// where in a release version the error would also include a defined error-code, with the messages themselves abstracted out
// to a moveable structure. Since this is a development tool, the messages are left in place (rather than being abstracted as
// they would in a release version) for self-documentation and diagnostic purposes.

// note: update plans include converting direct message output to defined error codes and message output.  But, for development
// purposes and clarity of code, I like the current inline messages, as they provide some self-documentation.  
//

#define cnnTry cnnErr cnnStatus = cnnErr::Ok; try{                                                 // basic try() block
#define cnnCatch }catch(char * sMessage) { if (sMessage) SetErrorMsg(sMessage); }               // basic catch() block, setting message
#define cnnCatchNC }catch(char * sMessage) { sMessage; }                                        // Development usage, i.e. outside of CNN (non-CNN Class)
#define cnnAssert(_x,_y) if (!((_x))) { cnnStatus = cnnErr::Err;  throw((char *) _y); }              // Basic assertion, setting error status
#define cnnAssertOk(_x,_y) if (_x != cnnOk) { cnnStatus = cnnErr::Err;  throw((char *) _y); }        // Assertion for calling a function that returns a cnnErr_t
#define cnnAssertNC(_x) if (!((_x))) { cnnStatus = cnnErr::Err;  throw((char *) NULL); }             // Development usage, non-CNN/No class version
#define cnnPass(_x) if (_x == cnnErr::Err) { cnnStatus = cnnErr::Err;  throw((char *) NULL); }            // Assert passthrough in chain cnnStatus_t functions, allowing
                                                                                                // original error message and Error code to come down the chain.
// Exception Handling error type
//
enum class cnnErr
{
    Ok       ,        // Ok = false to allow simple (!Function()) checks for ok/not ok.
    Err,

    // Error Codes Here (currently implemented as cnnErr only)

    Stop     ,       // Training stopped by user (i.e. not an error)
    MemError ,       // for example.
};

// Default CNNAccess, used for MSVC.  These are access functions that can be 
// changed for different processors and compilers.
//
// The default is MSVC on intel architecture.
//
class CNNAccess
{
private:
public:
    void Srand(unsigned int uiValue) { srand(uiValue); }
    double Rand1() {  return (double) rand()/32767.0; }  // return random double between 0 and 1
                                                         // note: using mod (i.e. %) introduces a bias
    unsigned int uiRand() {  return rand() & 0x7ffff; }   // return unsigned int rand
    CNNAccess() { }
};
// CNN class declaration

class CDevNN : private CNNAccess
{
protected:

    // Define maximum error nodes for error calculation. This is done for develpment purpose and, in a standalone/release situation, would otherwse
    // be allocated based on number of outputs.   For now, a practical value is used to help development and debugging.

    static const int kMaxErrorNodes    = 500000;

    // Define maximum hidden layers.  This is just a practical value for debugging, so that we can look at m_stLayers very easily in the
    // debugger.  Otherwise it is more difficult.  In a release situation, m_stLayers would be allocated and kMaxHiddenLayers would not apply.
    // In other words, kMaxHiddenLayers is defined for development purposes.

    static const int kMaxHiddenLayers  = 50000;      

    static const int kMaxErrMsgLength  = 200;
                                                                                            // Used to avoid multi-function throws, to keep local control
    // Set defaults for Learning Rate changes
    // (used #define because too much fighing with the compiler over const float/bool)

    static const bool       kdefRChangeable        ;    // Auto Learning Rate yes/no
    static const double     kdefLearningRate       ;    // How much to increase LR when on Auto
    static const double     kdefConvergeIncrease   ;    // How much to reduce LR on divergence when on Auto
    static const double     kdefDivergeMult        ;    // How long to weight before increasing LR
    static const int        kdefConvergeCount      ;    // Maximum leanring rate
    static const int        defMaxLearnRate        ;    // Minimum learning rate
    static const double     defMinLearnRate        ;    // Learn Rate to start
    static const int        defMinAdjustEpochs     ;    // Minimum Epoch count before adjust learning rate (somewhat arbitrary number)
    static const double     kDeftaultWeightMul     ;    // Default weight multiplier on FillWeights()

public:
    // Lock Enable for locking weights/nodes.  Just for specific, declarative access, i.e.  CNN::LockEnable:On
 
    enum class LockEnable
    {
        On,
        Off
    };

    // Activation Types

    enum class ActType
    {
        SoftMax     ,
        Relu        ,
        Sigmoid     ,
        TanH        ,
        Custom      ,   // TBD, but will call out to an overloaded function that can then be redirected by the owning object.
        None        ,   // Do nothing (no activation, derivative returns 1)  
        Input       ,   // Reference for Input Layer (has no effect)
        End         ,   // Signal for end of the NN structure
    };

    // Node Structure Type, to pass into the CNN constructor, defining the neural network. 
    //
    // i.e
    //   2  ,CNN::ActType::Input    ,
    //  15  ,CNN::ActType::TanH     ,
    //  25  ,CNN::ActType::Relu     ,
    //  5   ,CNN::ActType::SoftMax   ,      
    //  0   ,CNN::ActType::End       ,

    struct stLayerInput_t
    {
        int   iNodes;
        ActType  eActType;
    };

protected:
    const double    kMaxDouble              = DBL_MAX      ;    // These should be max and min <type>.  A generic FLT_MAX
    const double    kMinDouble              = -DBL_MAX     ;    // would certainly work just fine. 
    const double    UseCurrentWeight        = DBL_MAX      ;    // Used for weight/node locking with current weight, i.e. CNN::UseCurrentWeight
                                                           ;    // bounds checking, and any weight that trends to these values                                                          
                                                           ;    // is a sign of a NN out of control (exploding gradient or NAN, etc.)
    enum class Trend
    {
        Converge,       // Converging (errors going down)
        Diverge  ,      // Diverging (errors increasing)
        None     ,      // None -- probably a sign of an error, such as a NAN
    };

    // Structure for each layer, where memory and other values are stored.
    struct stLayer_t
    {
        int       iNodes        ;   // Number of input nodes
        int       iOutputNodes  ;   // Nodes in next layer
        ActType   eActType      ;   // Activation Type, i.e. RELU, Sigmoid, TanH, SoftMax
        double  * fWeights      ;   // Current set of weights
        double  * fBias         ;   // Current set of bias weights (bias is always 1)
        double  * fWDev         ;   // Latest Derivation for weights
        double  * fBDev         ;   // Latest Derication for biases
        double  * fWDevLast     ;   // Last derivatives for use with Momentum function
        double  * fBDevLast     ;
        double  * fIn           ;   // Input Values for each node, i.e. I1*W1 + I1*W2 + B2, or Sigmoid(value), etc. , or first Inputs
        double  * fDevCalc      ;   // Partial Derivative Accumulation from each output weight (i.e. output, or input to next right node) 
        double  * fWeightLock   ;   // Used to freeze weights at a specific value
        int       iWDevMem      ;   // Total size of WDev memory for later use
        int       iInMem        ;   // Total Size of Input Memory for later use
        int       iWRange       ;   // Weight Range (Nodes * Weights)
        int       iWBRange      ;   // (Weight + Bias) Range ((Nodes + 1) * Weights)

        long      iMemAllocated ;   // Total memory allocated, just for reference.
        double   (CDevNN::*Activate)(double);      // Activation function. Changes depending on type, Sigmoid, Relu, SoftMax, etc.
        double   (CDevNN::*Derivative)(double);    // Derivative/Gradient function.  Changes with layer activation type, i.e. Sigmoid, Relu, etc.
    };

// Purposely leave a lot exposed, so development tools can have more direct access.

protected:
    int           m_iEpoch;                             // Current training Epoch
                                                        // neural network initially trains, but off values can also cause out-of-control issues.
    int           m_iEpochs      = 0;   // Number of loops ("Epochs") through data set when training
    int           m_iDataSets    = 0;   // Number of data sets for each input 
    stLayer_t     m_stLayers[kMaxHiddenLayers] = {};    // kMaxHiddenLayers Used for development purposes.  This should otherwise be allocated memory.
    stLayer_t   * m_stLast;                             // Pointer to Output (i.e. last layer), for ease-of-use

    int           m_iLayers               = 0;

    bool          m_bLRChangeable           = kdefRChangeable       ;   // Auto Learning Rate yes/no
    double        m_fConvergeIncrease       = kdefConvergeIncrease  ;   // How much to increase LR when on Auto
    double        m_fDivergeMultiply        = kdefDivergeMult       ;   // How much to reduce LR on divergence when on Auto
    int           m_iConsecutiveConverge    = kdefConvergeCount     ;   // How long to weight before increasing LR
    double        m_fMaxLearnRate           = defMaxLearnRate       ;   // Maximum leanring rate
    double        m_fMinLearnRate           = defMinLearnRate       ;   // Minimum learning rate
    double        m_fLearningRate           = kdefLearningRate      ;   // Learn Rate to start
    int           m_iMinAdjustEpochs        = defMinAdjustEpochs    ;   // Minimum Epoch count before adjust learning rate (somewhat arbitrary number)
    double        m_fWeightMul              = kDeftaultWeightMul    ;   // Value to multiply weight rand() values.  Can make a difference on how fast the 

    char          m_sLastError[kMaxErrMsgLength] = {};
    Trend         m_eTrend;
    int           m_iInputs = 0;
    int           m_iOutputs = 0;
    double        m_fError[kMaxErrorNodes];             // kMaxErrorNodes Used for development purposes.  This should otherwise be allocated memory.
    double        m_fTotalError     ;
    double        m_fTrendError     ;
    bool          m_bSoftMax        = false;


    cnnErr TrainUpdateForward();                                  // Perform a single forward propagation based on specific inputs (m_fUpdateInputs) for an update process

    // Return update memory (input and output) for a Forward() pass (i.e. one dataset).  This is used by 
    // TrainUpdate() (during training) to check on various inputs and compare outputs. 
    // These are generally mapped to the memory allocated for the Input and Ouput layers, respectively, but
    // this should be assumed, and GetUpdateMem() used instead.

    void GetUpdateMem(double * & fInputs,double * & fOutputs) { fInputs = m_fUpdateInputs; fOutputs = m_fUpdateOutputs; }

    // Custom override activation functions.
    // Add more (i.e. ActivateCustom2(), DerivativeCustom2(), etc.) to add more custom values,
    // and add code to CreateMem() to direct to Custom2(),Custom3(), etc.

    virtual double ActivateCustom      (double fValue) { return fValue; } 
    virtual double DerivativeCustom    (double fValue) { return 1;      } 

private:
    const double * m_fpDataSet           = NULL  ;   // Location of the data sets
    double       * m_fpEOutputs          = NULL  ;   // Location of expected output for each training set.
    bool           m_bLockEnabled        = false ;
    double         m_fMomentum           = 0     ;
    const char   * m_sTrainMsg           = NULL  ;   // Message returned from TrainUpdate to pass back to owner if it aborts.
    unsigned int   m_uiRandSeed          = 1234  ;   // Anything until its replaced by the owning process.
    double         m_fLastTrendError             ;
    int            m_iConverge                   ;   // Convergence counter for changing the Learning Rate
    double       * m_fUpdateInputs       = NULL  ;   // Input and Output for TrainUpdate().  These are just references to the
    double       * m_fUpdateOutputs      = NULL  ;   //   allocated memory, but are left here in case this changes in the future.

    CDevString    m_lcs;                            // General, loose outputs strings, such as (m_lcs >> "This is epoch #" << iEpoch).s, etc.
                                                    // Used primarily for diagnostics, debug, and general information. 
    double        m_fSoftMaxSum                 ;   // Output Layer output Summation performed for Softmax during forward() function
    bool          m_bWeightInit         = false ;   // Weight Initialized yes/no.  Done on Train() call to make it idempotent
    void Initialize();
    cnnErr Alloc(int iIndex,double * & fMem,int iSize,const char * sName);
    cnnErr CreateMem();
    void FreeMem();
    bool CheckError() { return m_sLastError[0] != 0; }
    void SetErrorMsg(const char * sMsg) { m_sLastError[0] = 0; if (sMsg) { strncpy(m_sLastError,sMsg,kMaxErrMsgLength-1); m_sLastError[kMaxErrMsgLength-1] = 0; }}
    cnnErr ValidateStructure();
    void FillWeights(const double * fWeights = NULL);

    void AdjustError();
    void AdjustLearnRate(int iEpoch);
    void GetMinMax(double * fMem,int iLength,double & fMin,double & fMax);

    // Activation functions and their derivatives

    double ActivateSig      (double fValue) { return 1/(1+exp(-fValue));    }      // Sigmoid
    double DerivativeSig    (double fValue) { return fValue*(1-fValue);     } 
    double ActivateTanH     (double fValue) { return tanh(fValue);          }      // Tanh
    double DerivativeTanH   (double fValue) { return 1-fValue*fValue;       } 
    double ActivateRelu     (double fValue) { return fValue > 0 ? fValue : 0;                    }      // Relu
    double DerivativeRelu   (double fValue) { return fValue == 0 ? .1 : fValue < 0 ? 0 : 1;      }
    double DerivativeNone   (double fValue) { return 1;                     };     // Do Nothing
    double ActivateNone     (double fValue) { return fValue;                }; 
    double ActivateSoftMax  (double fValue);    // Soft max functions are in their own bodies, 
    double DerivativeSoftMax(double fValue);    //   mostly because of comments, lots of comments.
    double LossRMS          (double fOut,double feOut) { return 2*(fOut - feOut); }                       // Canonical form
    double LossSoftMax      (double fOut,double feOut) { return -1*feOut/fOut + (1-feOut)/(1-fOut); }     // Derivative of Entropy loss ($$ check this)                                                
 
    void    Forward(const double * & fpIn);       // N-Layer Forward Propogation
    void    Backward(double * & fpOut);     // N-Layer Backward Propogation
    void    ApplyDerivativesStd();
    void    ApplyDerivativesStdLock();
    void    ApplyDerivativesMom(); 
    double  (CDevNN::*Loss)(double,double);    // See notes in Backward()
    void    (CDevNN::*ApplyDerivatives)(void); // See notes in Train()
     
    virtual cnnErr TrainUpdate(const char * & sMsg) { return cnnErr::Ok; }   // Return a message to abort the training

public:
    // get/set functions.  Added as needed, since this is a development set of code, many things are referenced directly in the protected section
    // note: there are a few that need to be added.  For development, I put a lot of values for direct access in a protected section.  These
    // should righfully be here as inlines.  TBD

    inline void setWeightMul(double fMul) { m_fWeightMul = fMul <= 0 ? kDeftaultWeightMul : fMul; }
    inline Trend getTrand() { return m_eTrend; };
    inline const char * getTrendString() { return m_eTrend == Trend::Converge ? "Converging" : m_eTrend == Trend::Diverge ? "Diverging" : "Even"; };
    inline double getError() { return m_fTrendError; }         // Get overall error.  Could also add (int iOutput) to get error for each output.
    inline int getEpoch() { return m_iEpoch; }
    inline int setEpochs(int iEpochs) { m_iEpochs = iEpochs; } // A makeshift way to get the Train() function to end gracefully, or continue on
    inline bool    isSoftMax()     { return m_bSoftMax; }      // returns whether outputs are Softmax.  Can be useful for training update.
    inline double  *getErrorMem()  { return m_fError; }        // return error memory for each layer.
    inline double getLearningRate() { return m_fLearningRate; };   // Return current learning rate

    // The following are useful for changing run-time Train() training data in TrainUpdate()

    inline void setDataSets(int iDataSets) { m_iDataSets = iDataSets; } 
    inline void setTrainInputMem(const double * fpMem) { m_fpDataSet = fpMem; }
    inline void setTrainOutputMem(double * fpMem) { m_fpEOutputs = fpMem; }

    // Set the training input and output memory.  This can be used by TrainUpdate() to change how 
    // the training set is seen, to have the neural network focus on different parts of the
    // training data, or to give differing sets of training data altogether. 
    //
    // This can be paired with other diagnostic tools, such as graphically showing how weights change when the training data 
    // set is changed (DavCNN version, for example).

    inline void SetTrainingMem(const double * fIn,double *fOut) { m_fpDataSet = fIn; m_fpEOutputs = fOut; }

public:
    CDevNN(unsigned int uiRandSeed,LockEnable eLockEnable,stLayerInput_t * & stLayers, double * fWeights = NULL);
    ~CDevNN();

    void SetMomentum(double fMomentum) { m_fMomentum = fMomentum; }
    cnnErr Train(int iEpochs,int iDataSets,double * fpDataSet,double * fpEOutputs);
    void SetLearnRate(double fRate) { if (fRate > 0) m_fLearningRate = fRate; }
    void SetRateChange(bool bAutoChange,double fConvergeIncrease = kdefConvergeIncrease,double fDivergeMultiply = kdefDivergeMult,double fMaxLearnRate = defMaxLearnRate,int iConvergeCount = kdefConvergeCount);
    void SetRateChangeBounds(double fMin,double fMax,int iRateChangeDelay);
    char * GetErrorMsg() { return (char *) &m_sLastError; }
    cnnErr GetMinMaxWeight(int iLayer,double & fMin,double & fMax);
    cnnErr GetMinMaxBias(int iLayer,double & fMin,double & fMax);
    cnnErr LockWeight(int iLayer,int iNode,int iOutputNode,double fWeight);
    cnnErr UnlockWeight(int iLayer,int iNode,int iOutputNode);
};
