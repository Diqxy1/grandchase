========================================================================
    정적 라이브러리 : GCDeviceLib 프로젝트 개요
========================================================================

- GCDeviceLib는 게임내의 Device(Texture,Sound,Motion,Model,Font,Sprite) 등을 관리하는 Module입니다

- 이름에 Device가 붙지 않은 (GCUITexture, GCUIFont) 는 DeviceManager를 통해서 만들지 않고, 바로 new해서 사용합니다
내부적으로 Device를 가지며, 동일 (Texture 혹은 Font)를 다양하고 편리하게 사용할 수 있도록 하기 위한 class입니다
