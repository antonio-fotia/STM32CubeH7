/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINVIEWBASE_HPP
#define MAINVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen/mainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/scrollers/ScrollWheel.hpp>
#include <gui/containers/optionButton.hpp>

#include <gui/containers/popup.hpp>
class mainViewBase : public touchgfx::View<mainPresenter>
{
public:
    mainViewBase();
    virtual ~mainViewBase() {}
    virtual void setupScreen();

    virtual void moduleSelectorWheelUpdateItem(optionButton& item, int16_t itemIndex)
    {
        // Override and implement this function in main
    }

    /*
     * Virtual Action Handlers
     */
    virtual void popUpBackPressed()
    {
        // Override and implement this function in main
    }

    virtual void popUpStartPressed()
    {
        // Override and implement this function in main
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box background;
    touchgfx::Image stMotto;
    touchgfx::ScrollWheel moduleSelectorWheel;
    touchgfx::DrawableListItems<optionButton, 5> moduleSelectorWheelListItems;

    popup infoPopUp;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<mainViewBase, touchgfx::DrawableListItemsInterface*, int16_t, int16_t> updateItemCallback;
    touchgfx::Callback<mainViewBase> infoPopUpBackButtonPressedCallback;
    touchgfx::Callback<mainViewBase> infoPopUpStartButtonPressedCallback;

    /*
     * Callback Handler Declarations
     */
    void updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex);
    void infoPopUpBackButtonPressedCallbackHandler();
    void infoPopUpStartButtonPressedCallbackHandler();

};

#endif // MAINVIEWBASE_HPP
