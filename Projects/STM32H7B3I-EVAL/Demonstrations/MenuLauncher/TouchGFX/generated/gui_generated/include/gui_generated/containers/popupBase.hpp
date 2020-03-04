/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef POPUPBASE_HPP
#define POPUPBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class popupBase : public touchgfx::Container
{
public:
    popupBase();
    virtual ~popupBase() {}
    virtual void initialize();

    /*
     * Custom Trigger Callback Setters
     */
    void setBackButtonPressedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->backButtonPressedCallback = &callback;
    }
    void setStartButtonPressedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->startButtonPressedCallback = &callback;
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Custom Trigger Emitters
     */
    virtual void emitBackButtonPressedCallback()
    {
        if (backButtonPressedCallback && backButtonPressedCallback->isValid())
        {
            this->backButtonPressedCallback->execute();
        }
    }
    virtual void emitStartButtonPressedCallback()
    {
        if (startButtonPressedCallback && startButtonPressedCallback->isValid())
        {
            this->startButtonPressedCallback->execute();
        }
    }

    /*
     * Member Declarations
     */
    touchgfx::Image popUpOverlay;
    touchgfx::TextArea headline;
    touchgfx::TextAreaWithOneWildcard infoText;
    touchgfx::IconButtonStyle< touchgfx::ClickButtonTrigger > backButton;
    touchgfx::IconButtonStyle< touchgfx::ClickButtonTrigger > startButton;

    /*
     * Wildcard Buffers
     */
    static const uint16_t INFOTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar infoTextBuffer[INFOTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<popupBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Custom Trigger Callback Declarations
     */
    touchgfx::GenericCallback<>* backButtonPressedCallback;
    touchgfx::GenericCallback<>* startButtonPressedCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // POPUPBASE_HPP
