void
spiral(Monitor *mon) {
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;
	int s = 0;

	for(n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;
	
	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;
	
	for(i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if(i < n - 1) {
				if(i % 2)
					nh /= 2;
				else
					nw /= 2;
				if((i % 4) == 2 && !s)
					nx += nw;
				else if((i % 4) == 3 && !s)
					ny += nh;
			}
			if((i % 4) == 0) {
				if(s)
					ny += nh;
				else
					ny -= nh;
			}
			else if((i % 4) == 1)
				nx += nw;
			else if((i % 4) == 2)
				ny += nh;
			else if((i % 4) == 3) {
				if(s)
					nx += nw;
				else
					nx -= nw;
			}
			if(i == 0)
			{
				if(n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			}
			else if(i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

void
monocle(Monitor *m)
{
	unsigned int n = 0;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;
	for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx, m->wy + my, mw - (2*c->bw) + (n > 1 ? gappx : 0), h - (2*c->bw), 0);
			if (my + HEIGHT(c) < m->wh)
				my += HEIGHT(c);
		} else {
			h = (m->wh - ty) / (n - i);
			resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
			if (ty + HEIGHT(c) < m->wh)
				ty += HEIGHT(c);
		}
}
