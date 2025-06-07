#pragma once

int main()
{
    Eepy::IApplication& pApp = *Eepy::EntryApplication();


    Display* display = XOpenDisplay(NULL);

    Window window = XCreateSimpleWindow(display, 
                                        DefaultRootWindow(display),
                                        0, 0,
                                        800, 600, 
                                        1,BlackPixel(display, 0), WhitePixel(display, 0));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    bool running = true;
    while (running)
    {
        while (XPending(display) > 0)
        {
            XEvent event;
            XNextEvent(display, &event);
            switch (event.type)
            {
                case KeyPress:
                    running = false;
                    break;
            }
        }

        pApp.Update();
    }

    

    pApp.Initialize();
    pApp.Destroy();
}
