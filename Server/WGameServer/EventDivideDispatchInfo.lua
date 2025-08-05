--[[
DB�� �Ѳ����� ���� ��û�� ������ Just In Time �̺�Ʈ�� ���� ���ߵǾ���
���� �ð����� DB ť ũ�⸦ Ȯ���Ͽ� ���� �����忡�� DB ������� ť���ϴ�
�̺�Ʈ�� �����Ͽ� ������� �����ϴ� ����� �Ѵ�

���� ���� �����忡�� DB �����忡 �����ϴ� �κ��� �����Ǿ� �ִ�
�ݴ��� ���� �� ���� �����̴�
--]]


-- UserEvent_def.h�� �̺�Ʈ ���� ��Ī��Ű�� �۾��� �ڵ忡�� ���� ����� �Ѵ�
DB_EVENT_JUST_IN_TIME_REWARD_REQ = 0

EventInfo =
{
    {
        ProcessTimeInterval = 5 * 1000,
        EventID = "DB_EVENT_JUST_IN_TIME_REWARD_REQ",
        QueueSizeThreshold = 600, -- �߰� ť���� ���� ����(���⼭ ������ DB queue size �� �ʰ����� �ʰ� �߰�ť����)
        ProcessCount = 12, -- �ѹ��� ó���� �ִ� ���� �̺�Ʈ��(DBQueueSize ������ �켱�Ͽ� ó����)
    },
    {
        ProcessTimeInterval = 5 * 1000,
        EventID = "DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ",
        QueueSizeThreshold = 600, -- �߰� ť���� ���� ����(���⼭ ������ DB queue size �� �ʰ����� �ʰ� �߰�ť����)
        ProcessCount = 12, -- �ѹ��� ó���� �ִ� ���� �̺�Ʈ��(DBQueueSize ������ �켱�Ͽ� ó����)
    },
}
