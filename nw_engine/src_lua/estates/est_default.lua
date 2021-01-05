print("\n--==<DEFAULT_ESTATE>==--")

ent0 = nil
ent1 = nil

function init()
end

function update()
end

function on_enable()
	ent0 = ent_sys.create_ent("entity_15")
	ent1 = ent_sys.get_ent("entity_15")
	ent_sys.set_ent_enabled(ent0, true)
	
	gcmp0 = cmp_sys.create_cmp(ent0, "cmp_graphics_2d")
	tcmp0 = cmp_sys.create_cmp(ent0, "cmp_transform_2d")
	scmp0 = cmp_sys.create_cmp(ent0, "cmp_script_lua")

	ent_sys.set_ent_enabled(ent0, true)
	ent_sys.set_ent_enabled(ent1, true)
end

function on_disable()
end

print("\n--==<DEFAULT_ESTATE>==--\n")