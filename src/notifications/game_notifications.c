#include "game_notifications.h"
/**
 * Takes a RingaBuf pointer and queues the passed text to it.
 * @see handleRoom_Enemies()
 * @see handleRoom_Boss()
 * @param text The contents of the notification.
 * @param time The display time in milliseconds (unused, for now)
 * @param color The index of color to use for the notification drawing.
 * @param rb_notifications The Ringabuf to push the notification to.
 */
void enqueue_notification(char *text, int time, int color, RingaBuf* rb_notifications)
{
    Notification notif = {0};

    sprintf(notif.buf, "%s", text);
    notif.color = color;

#ifndef _WIN32
    log_tag("debug_log.txt", "[DEBUG]", "%s():    pushing {%li} bytes to ringbuf", __func__, sizeof(notif));
#else
    log_tag("debug_log.txt", "[DEBUG]", "%s():    pushing {%lli} bytes to ringbuf", __func__, sizeof(notif));
#endif
    bool rb_res = rb_push(*rb_notifications, notif);


    /* Log ring status buffer, from oldest to newest notification.
    Notification* newest_notif = NULL;
    Notification* oldest_notif = NULL;

    if (!rb_notifications->is_full) {
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Logging up from 0 to head: { %" PRIu32 " }", __func__, rb_notifications->head);
        for (int i = 0; i < (rb_notifications->head / sizeof(Notification)); i++) {
            Notification* read_notif = (Notification*) &(rb_notifications->data[i * sizeof(Notification)]);
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Notification: [%s] Color: [%" PRId8 "]", __func__, read_notif->buf, read_notif->color);
        }
        oldest_notif = (Notification*) &(rb_notifications->data[0]);
        newest_notif = (Notification*) &(rb_notifications->data[rb_notifications->head - (sizeof(Notification))]);
    } else {
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Logging up from head+1 { %" PRIu32 " } to size { %" PRIu32 " }, then from 0 to head.", __func__, (rb_notifications->head / sizeof(Notification)) +1, rb_notifications->capacity / sizeof(Notification));
        for (size_t i = (rb_notifications->head / sizeof(Notification)) +1; i < (rb_notifications->capacity / sizeof(Notification)); i++) {
            Notification* read_notif = (Notification*) &(rb_notifications->data[i * sizeof(Notification)]);
            log_tag("debug_log.txt", "[DEBUG]", "%s():    [%li] H+1->sz Notification: [%s] Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
        }
        for (size_t i = 0; i < (rb_notifications->head / sizeof(Notification)); i++) {
            Notification* read_notif = (Notification*) &(rb_notifications->data[i * sizeof(Notification)]);
            log_tag("debug_log.txt", "[DEBUG]", "%s():    [%li] 0->H Notification: [%s] Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
        }

        size_t newest_offset = (rb_notifications->head == 0 ? ((NOTIFICATIONS_RINGBUFFER_SIZE-1)* sizeof(Notification)) : (rb_notifications->head - sizeof(Notification)));
        newest_notif = (Notification*) &(rb_notifications->data[newest_offset]);
        oldest_notif = (Notification*) &(rb_notifications->data[(rb_notifications->head)]);

    }

    log_tag("debug_log.txt", "[DEBUG]", "%s():    Newest Notification: [%s] Color: [%" PRId8 "]", __func__, newest_notif->buf, newest_notif->color);
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Oldest Notification: [%s] Color: [%" PRId8 "]", __func__, oldest_notif->buf, oldest_notif->color);
    */

#ifndef _WIN32
    if (!rb_res) log_tag("debug_log.txt", "[DEBUG]", "%s():    rb push failed. Head: { %" PRIu32 " } Tail: { %" PRIu32 " } Capacity: {%li}", __func__, rb_get_head(*rb_notifications), rb_get_tail(*rb_notifications), rb_get_capacity(*rb_notifications));
#else
    if (!rb_res) log_tag("debug_log.txt", "[DEBUG]", "%s():    rb push failed. Head: { %" PRIu32 " } Tail: { %" PRIu32 " } Size: {%lli}", __func__, rb_get_head(*rb_notifications), rb_get_tail(*rb_notifications), rb_get_capacity(*rb_notifications));
#endif

    /*
    wprintw(w, "\n  %s", text);
    wrefresh(w);
    //refresh();
    napms(time);
    */
}
