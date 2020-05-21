import mountainhuts.*;

import static org.junit.Assert.*;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.junit.Test;

public class ExampleTest {

	@Test
	public void testAll() {

		Region r = Region.fromFile("Piemonte", "mountain_huts.csv");
		assertNotNull("Missing region", r);

		assertNotNull("Missing region name", r.getName());
		assertNotNull("Missing range name", r.getAltitudeRange(0));
		r.setAltitudeRanges("0-1000", "1001-2000", "2001-3000");
		assertEquals("Wrong value in range", "1001-2000", r.getAltitudeRange(1500));

		Collection<Municipality> municipalities = r.getMunicipalities();
		assertNotNull("Missing municipalities", municipalities);
		assertEquals("Wrong number of municipalities", 94, municipalities.size());

		Collection<MountainHut> mountainHuts = r.getMountainHuts();
		assertNotNull("Missing mountain huts", mountainHuts);
		assertEquals("Wrong number of mountain huts", 167, mountainHuts.size());

		Map<String, Long> res1 = r.countMunicipalitiesPerProvince();
		assertNotNull("Missing count of municipalities per province", res1);
		assertEquals("Wrong number of municipalities in province of Torino", Long.valueOf(24), res1.get("TORINO"));

		Map<String, Map<String, Long>> res2 = r.countMountainHutsPerMunicipalityPerProvince();
		assertNotNull("Missing count of mountain huts per municipality per province", res2);
		Map<String, Long> resTo = res2.get("TORINO");
		assertNotNull("Missing count of mountain huts in province of Torino", resTo);

		Map<String, Long> res3 = r.countMountainHutsPerAltitudeRange();
		assertNotNull("Missing count of mountain huts per altitude range", res3);
		assertEquals("Wrong number of mountain huts in altitude range 0-1000", Long.valueOf(22), res3.get("0-1000"));

		Map<String, Integer> res4 = r.totalBedsNumberPerProvince();
		assertNotNull("Missing total beds number per province", res4);
		assertEquals("Wrong number of beds number in province of Torino", Integer.valueOf(953), res4.get("TORINO"));

		Map<String, Optional<Integer>> res5 = r.maximumBedsNumberPerAltitudeRange();
		assertNotNull("Missing maximum beds number per altitude range", res5);
		assertEquals("Wrong number of maximum beds number in altitude range 0-1000", Integer.valueOf(27),
				res5.get("0-1000").get());

		Map<Long, List<String>> res6 = r.municipalityNamesPerCountOfMountainHuts();
		assertNotNull("Missing set of municipality names per count of mountain huts", res6);
		assertEquals("Wrong number of municipalities per count 1", 61, res6.get(1L).size());

	}

}
